// Implementation of the Agent ADT by Raghav Agarwal (z5394767)
// Start date: July 20th 2022
// Date: July 30th finally found the solution to stage 3
// This program implements Agent strategies which include:
//      A Randomised move strategy
//      A Cheapest least visited strategy
//      A Dfs strategy
//      and a Shortest path strategy

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Agent.h"
#include "Map.h"

#define INFINITY 9999
#define LEAST_TURNS_PATH 3

// This struct stores information about an individual agent and can be
// used to store information that the agent needs to remember.
struct agent {
    char *name;                 // this is the name of the agent
    int startLocation;          // saves the agents start location
    int location;               // saves agents current location
    int maxStamina;             // max stamina
    int stamina;                // current stamina
    int strategy;               // agents current strategy
    Map map;                    // saves the map in the agent
    int *moves;                 // To keep track of agent moves to all cities
    int totalMoves;             // The totalMoves that agent makes overall
    bool *visitedArray;         // saves the places that agent has visited in order
    Road *prevTracker;          // from is where we came from and to is where we went to
    int initialStrategy;        // used in case agents strategy changes to shortest path one
    int givenThiefLocation;     // when informant found this saves the theifs current location
};

static Move shortestPathFinder(Agent agent, Map m);
int lengthLessThanStaminaRoads(Agent agent, Road roads[], int numRoads, Road legalRoads[]);
void AddMoveToPrevArray(Map m, Agent agent, int from, int to, int length);
int MapNotVisitRoads(Agent agent, Map m, Road allPosRoads[], int numRoads, Road roads[]);
static Move chooseDfs(Agent agent, Map m);
int minMoves(Agent agent, Road legalRoads[], int numLegalRoads, Road leastMovesArray[]);
int minLength (Map m, Road legalRoads[], int numLegalRoads, Road leastLengthArray[]);
void AddMoveToCityandTotal(Agent agent, int city);
static Move cheapestLeastVisited(Agent agent, Map m);
static Move chooseRandomMove(Agent agent, Map m);
static int filterRoads(Agent agent, Road roads[], int numRoads, Road legalRoads[]);
void PrintPath(Map m, int parent[], int d, int checkedChild[], int checkedParent[], bool check, int finalPath[]);
int minDistance (Map m, int visited[], int distance[]);
void dijkstraModed(Map m, Agent agent, int finalPath[]);

// This funtion we are getting from Map.c
int **MapGetAllRoads(Map m);

/**
 * Creates a new agent
 */
Agent AgentNew(int start, int stamina, int strategy, Map m, char *name) {
    if (start >= MapNumCities(m)) {
        fprintf(stderr, "error: starting city (%d) is invalid\n", start);
        exit(EXIT_FAILURE);
    }
    
    Agent agent = malloc(sizeof(struct agent));
    if (agent == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }
    
    agent->startLocation = start;
    agent->location = start;
    agent->maxStamina = stamina;
    agent->stamina = stamina;
    agent->strategy = strategy;
    agent->initialStrategy = strategy;
    agent->map = m;
    agent->name = strdup(name);
    agent->moves = calloc(MapNumCities(m), sizeof(int));
    agent->totalMoves = 0;
    agent->visitedArray = malloc(MapNumCities(m) * sizeof(bool));
    for (int i = 0; i < MapNumCities(m); i++) {
        agent->visitedArray[i] = false;
    }
    agent->prevTracker = malloc(MapNumCities(m) * MapNumCities(m) * MapNumCities(m) * sizeof(Road));
    for (int i = 0; i < MapNumCities(m) * MapNumCities(m) * MapNumCities(m); i++) {
        agent->prevTracker[i].to = 0;
        agent->prevTracker[i].from = 0;
        agent->prevTracker[i].length = 0;
    }

    // Because currently we don't have a location of the theif and no city index or array used in the program has num -3
    agent->givenThiefLocation = -3;
    
    // Agent assumed to visit the start city once
    AddMoveToCityandTotal(agent, agent->location);

    return agent;
}

/**
 * Frees all memory associated with the agent
 */
void AgentFree(Agent agent) {
    free(agent->name);
    free(agent->moves);
    free(agent->visitedArray);
    free(agent->prevTracker);
    free(agent);
}

////////////////////////////////////////////////////////////////////////
// Making moves

/**
 * Calculates the agent's next move
 */
Move AgentGetNextMove(Agent agent, Map m) {
    switch (agent->strategy) {
        case STATIONARY:             return (Move){agent->location, 0};
        case RANDOM:                 return chooseRandomMove(agent, m);
        case CHEAPEST_LEAST_VISITED: return cheapestLeastVisited(agent, m);
        case DFS:                    return chooseDfs(agent, m);
        case LEAST_TURNS_PATH:       return shortestPathFinder(agent, m);
        default:
            printf("error: strategy not implemented yet\n");
            exit(EXIT_FAILURE);
    }
}

/**  
 *  This function will find the shortest path when an informant is found from agents current location to theif current* location
 */
static Move shortestPathFinder(Agent agent, Map m) {
    // Checks if the agent location has been set properly
    assert(agent->givenThiefLocation > -1);

    Move move;
    Road *roads = malloc(MapNumCities(m) * sizeof(Road));
    Road *legalRoads = malloc(MapNumCities(m) * sizeof(Road));

    // Get all roads to adjacent cities
    int numRoads = MapGetRoadsFrom(m, agent->location, roads);

    // Filter out roads that the agent does not have enough stamina for
    int numLegalRoads = filterRoads(agent, roads, numRoads, legalRoads);

    // checks if we don't have enough stamina for any city next to us
    if (numLegalRoads == 0) {
        move = (Move) {agent->location, 0};
        agent->visitedArray[agent->location] = true;
        AddMoveToCityandTotal(agent, agent->location);
        free(roads);
        free(legalRoads);
        return move;
    }

    // This array is being used to save the path that the agent has to follow for the shortest path to thief
    int finalPath[MapNumCities(m)];
    for (int i = 0; i < MapNumCities(m); i++) {
        finalPath[i] = -4;
    }

    // This is the function that will calculate that final path
    dijkstraModed(m, agent, finalPath);

    // This is the case when the agent need to rest
    if (finalPath[1] == -2) {
        move = (Move) {agent->location, 0};
        agent->visitedArray[agent->location] = true;
        AddMoveToCityandTotal(agent, agent->location);
        free(roads);
        free(legalRoads);
        return move;
    }

    // Gets all roads on the map
    int **AllRoads = MapGetAllRoads(m);

    // Final Path index 0 has agents current location and index 1 has agents next move
    move = (Move){finalPath[1], AllRoads[finalPath[0]][finalPath[1]]};

    // To send agent back to his initial strategy when the location we wanted to go has been reached
    if (finalPath[1] == agent->givenThiefLocation) {
        agent->strategy = agent->initialStrategy;
    }

    // setting things up if the agent strategy is DFS
    agent->visitedArray[finalPath[0]] = true;
    agent->visitedArray[finalPath[1]] = true;
    AddMoveToPrevArray(m, agent, finalPath[0], finalPath[1], AllRoads[finalPath[0]][finalPath[1]]);

    // setting things up if the agent strategy is CHEAPEST_LEAST_VISITED
    AddMoveToCityandTotal(agent, finalPath[1]);

    free(roads);
    free(legalRoads);
    return move;
}

// find the minimum moves required to make the next move to next city
int minDistance (Map m, int visited[], int distance[]) {
    int min = INFINITY;
    int min_index = -1;
    int v;
    for (v=0; v< MapNumCities(m); v++) {
        if (visited[v] == 0 && distance[v] < min) {
            min = distance[v];
            min_index = v;
        }
    }
    if (min == INFINITY) {
        return min;
    }
    return min_index;
}

// This function is used to find the shortest path to the thief and populate the find path array with that path
void dijkstraModed(Map m, Agent agent, int finalPath[]) {
    // Getting all roads on the map in AllRoads1 array
    int **AllRoads1 = MapGetAllRoads(m);

    // an array similar to AllRoads1 but with all places with length > 0 equalling to 1
    int **AllRoads = malloc(MapNumCities(m) * sizeof(int*));

    // setting all value in the AllRoads array to 0
    for (int i = 0; i < MapNumCities(m); i++) {
      AllRoads[i] = calloc(MapNumCities(m), sizeof(int));
    }

    // Initialzing the AllRoads array with all road lenghts equalling to 1
    for (int i = 0; i < MapNumCities(m); i++) {
        for (int j = 0; j < MapNumCities(m); j++) {
            if (AllRoads1[i][j] != 0) {
                AllRoads[i][j] = 1;
            }
        }
    }

    int moves[MapNumCities(m)];         // keeps the data of minimum number of moves required to reach a city
    int distance[MapNumCities(m)];      // keeps the data of minimum number of distance required to reach a city
    int visited[MapNumCities(m)];       // a check if the city has already been visited
    int pathlength[MapNumCities(m)];    // keeps the data of minimum number of pathlength required to reach a city
    int parent[MapNumCities(m)];        // tell us where we came from to reach the city at a certain index
    int stamina[MapNumCities(m)];       // keeps a track of the stamina left after making the move to the city in that index
    int u;                              // City from
    int v;                              // City to 
    int checkedChild[MapNumCities(m)];  // helps us identify the area that the agents need to rest at
    int checkedParent[MapNumCities(m)]; // keeps the data of the place that the agent was visiting but did not have enough stamina to do so
    int checked = 0;
    bool check = false;

    // Initialzing all of the above arrays with appropriate values
    for (int i = 0; i < MapNumCities(m); i++) {
        stamina[i] = agent->stamina;
        moves[i] = INFINITY;
        distance[i] = INFINITY;
        visited[i] = 0;
        pathlength[i] = 0;
        parent[i] = -1;
        checkedParent[i] = -2;
        checkedChild[i] = -2;
    }

    // Moves and distance required to reach agents current location is 0
    moves[agent->location] = 0;
    distance[agent->location] = 0;

    for (int count = 0; count < MapNumCities(m)-1; count++) {
        u = minDistance(m, visited, moves);

        // if u is INFINITY then there was no city left to possibly visit from this location on the map hence all possible paths found
        if (u == INFINITY) {
            break;
        }

        // setting visited to 1 from this city 
        visited[u] = 1;

        for (v = 0; v < MapNumCities(m); v++) {

            // checks if this city is not visited, there is a road to it from u, 
            // if required moves to get to it is less than the moves required to get to it from other city and
            // if stamina is enough to make the move
            if (visited[v] == 0 && AllRoads[u][v] != 0 && moves[u] + AllRoads[u][v] < moves[v] && stamina[u] >=  AllRoads1[u][v]) {
                parent[v] = u;
                stamina[v] = stamina[u] - AllRoads1[u][v];
                pathlength[v] = pathlength[parent[v]] + 1;
                moves[v] = moves[u] + AllRoads[u][v];
            } 
            // Same as above if statement just checks if the stamina is not enough to make the move then are 2 moves a cheaper option
            // if yes then make the move
            else if (visited[v] == 0 && AllRoads[u][v] != 0 && moves[u] + AllRoads[u][v] + 1 <= moves[v] && stamina[u] < AllRoads1[u][v]) {
                // checkedparent and checkedchild arrays are there to check where we will need to rest
                checkedParent[checked] = u;
                checkedChild[checked] = v;
                checked++;
                check = true;
                stamina[v] = agent->maxStamina - stamina[u];
                parent[v] = u;
                pathlength[v] = pathlength[parent[v]] + 2;
                moves[v] = moves[u] + AllRoads[u][v];
            } 
            // two exactly same statements as above just instead of length being 1 now length is the actual length between the
            // cities
            else if (visited[v] == 0 && AllRoads[u][v] != 0 && distance[u] + AllRoads1[u][v] < distance[v] && stamina[u] >=  AllRoads1[u][v]) {
                parent[v] = u;
                stamina[v] = stamina[u] - AllRoads1[u][v];
                pathlength[v] = pathlength[parent[v]] + 1;
                distance[v] = distance[u] + AllRoads[u][v];
            } else if (visited[v] == 0 && AllRoads[u][v] != 0 && distance[u] + AllRoads1[u][v] + 1 <= distance[v] && stamina[v] < AllRoads1[u][v]) {
                // checkedparent and checkedchild arrays are there to check where we will need to rest
                checkedParent[checked] = u;
                checkedChild[checked] = v;
                checked++;
                check = true;
                stamina[v] = agent->maxStamina - stamina[u];
                parent[v] = u;
                pathlength[v] = pathlength[parent[v]] + 2;
                distance[v] = distance[u] + AllRoads[u][v];
            }
            
        }
    }
    PrintPath(m, parent, agent->givenThiefLocation, checkedChild, checkedParent, check, finalPath);
    for (int r=0; r<MapNumCities(m); r++) {
        free(AllRoads[r]);
    }
    free(AllRoads);
}

// This function takes the parent array decrypts it and populates the finalPath array 
// with the path that has been found in DijkstaModed
void PrintPath(Map m, int parent[], int d, int checkedChild[], int checkedParent[], bool check, int finalPath[]) {

    // when decryption has reached the agentLocation which has parent -1 then the recursion stops and the finalPath array is ready
    if (parent[d] == -1) {
        for (int i = 0; i < MapNumCities(m); i++) {
            if (finalPath[i] == -4) {
                finalPath[i] = d;
                break;
            }
        }
        return;
    }

    PrintPath(m, parent, parent[d], checkedChild, checkedParent, check, finalPath);

    // Checks if the current two cities are the one that the agent had to rest between and if agent did have to rest we populate
    // the finalPath array with the value -2 indicating a rest
    for (int i =0; i < MapNumCities(m); i++) {
        if (check == true && d==checkedChild[i] && parent[d] == checkedParent[i]) {
            for (int j = 0; j < MapNumCities(m); j++) {
                if (finalPath[j] == -4) {
                    finalPath[j] = -2;
                    break;
                }
            }
        }
    }

    for (int i = 0; i < MapNumCities(m); i++) {
        if (finalPath[i] == -4) {
            finalPath[i] = d;
            break;
        }
    }
}

// Finds all the roads that haven't already been visited using the agent->visitedArray 
int MapNotVisitRoads(Agent agent, Map m, Road allPosRoads[], int numRoads, Road roads[]) {
    int num = 0;
    for (int i = 0; i < numRoads; i++) {
        if (agent->visitedArray[allPosRoads[i].to] == false) {
            roads[num] = allPosRoads[i];
            num++;
        }
    }
    return num;
}

// This adds the move to the previous array to keep track of the path agent has followed
void AddMoveToPrevArray(Map m, Agent agent, int from, int to, int length) {
    for (int i = 0; i < MapNumCities(m) * MapNumCities(m) * MapNumCities(m); i++) {
        if (agent->prevTracker[i].from == 0 && agent->prevTracker[i].to == 0 && agent->prevTracker[i].length == 0) {
            agent->prevTracker[i].from = from;
            agent->prevTracker[i].to = to;
            agent->prevTracker[i].length = length;
            break;
        }
    }
}

// Finds all the roads that have length that is less than the agents current stamina
int lengthLessThanStaminaRoads(Agent agent, Road roads[], int numRoads, Road legalRoads[]) {
    int num = 0;
    for (int i = 0; i < numRoads; i++) {
        if (roads[i].length <= agent->stamina) {
            legalRoads[num] =roads[i];
            num++;
        }
    }
    return num;
}

// This is the function to impletment the DFS strategy
static Move chooseDfs(Agent agent, Map m) {
    agent->strategy = DFS;
    agent->initialStrategy = DFS;

    Road *allPosRoads = malloc(MapNumCities(m) * sizeof(Road));
    Road *roads = malloc(MapNumCities(m) * sizeof(Road));
    Move move;

    agent->visitedArray[agent->location] = true;

    // checking if all cities have already been visited
    int checker = 0;
    for (int i = 0; i < MapNumCities(m); i++) {
        if (agent->visitedArray[i] == true) {
            checker++;
        }
    }

    // If all have been visited then do this
    if (checker == MapNumCities(m)) {
        // reseting visiting array as now all cities have been visited
        for (int i = 0; i < MapNumCities(m); i++) {
            agent->visitedArray[i] = false;
        }

        // reseting prevTracker array as now all cities have been visited
        for (int i = 0; i < MapNumCities(m) * MapNumCities(m) * MapNumCities(m); i++) {
            agent->prevTracker[i].to = 0;
            agent->prevTracker[i].from = 0;
            agent->prevTracker[i].length = 0;
        }

        // Then calling the function again to find the next city now from agents current location
        chooseDfs(agent, m);
    }

    // Gets all the adjacent cities to thisNode
    int numRoads1 = MapGetRoadsFrom(m, agent->location, allPosRoads);

    // Finds all the roads that haven't already been visited
    int numRoads = MapNotVisitRoads(agent, m, allPosRoads, numRoads1, roads);

    // if from current road there is no other road that hasn't already been visited then the agent starts to trace back his path back
    // and find the next possible city that hasn't been visited
    if (numRoads == 0) {
        int i = 0;
        while (i < MapNumCities(m) * MapNumCities(m) * MapNumCities(m)) {
            if (agent->prevTracker[i].to == 0 && agent->prevTracker[i].from == 0 && agent->prevTracker[i].length == 0) {
                break;
            }
            i++;
        }
        if (agent->prevTracker[i-1].length > agent->stamina) {
            move = (Move){agent->location, 0};
            AddMoveToCityandTotal(agent, agent->location);
            free(allPosRoads);
            free(roads);
            return move;
        }
        move = (Move){agent->prevTracker[i-1].from, agent->prevTracker[i-1].length};
        AddMoveToCityandTotal(agent, agent->prevTracker[i-1].from);
        agent->prevTracker[i-1].to = 0;
        agent->prevTracker[i-1].from = 0;
        agent->prevTracker[i-1].length = 0;
        free(allPosRoads);
        free(roads);
        return move;
    } else {
        // remeber priority is the city ID and already checked that this city hasn't been visited before
        if (roads[0].length <= agent->stamina) {
            move = (Move){roads[0].to, roads[0].length};
            agent->visitedArray[roads[0].to] = true;
            AddMoveToCityandTotal(agent, roads[0].to);
            AddMoveToPrevArray(m, agent, agent->location, roads[0].to, roads[0].length);
            free(allPosRoads);
            free(roads);
            return move;
        } 
    
    // if can't move to the least index then stay at the same location
    move = (Move){agent->location, 0};
    agent->stamina = agent->maxStamina;
    AddMoveToCityandTotal(agent, agent->location);
    free(allPosRoads);
    free(roads);
    return move;
    }
}

static Move cheapestLeastVisited(Agent agent, Map m) {
    agent->strategy = CHEAPEST_LEAST_VISITED;
    agent->initialStrategy = CHEAPEST_LEAST_VISITED;

    Road *roads = malloc(MapNumCities(m) * sizeof(Road));
    Road *legalRoads = malloc(MapNumCities(m) * sizeof(Road));
    Move move;

    // Get all roads to adjacent cities
    int numRoads = MapGetRoadsFrom(m, agent->location, roads);

    // Filter out roads that the agent does not have enough stamina for
    int numLegalRoads = filterRoads(agent, roads, numRoads, legalRoads);

    // If there is no road that the D can take
    if (numLegalRoads == 0) {
        move = (Move){agent->location, 0};
        agent->stamina = agent->maxStamina;
        AddMoveToCityandTotal(agent, agent->location);
        free(roads);
        free(legalRoads);
        return move;
    }

    // If there is only one road the D can take
    if (numLegalRoads == 1) {
        move = (Move){legalRoads[0].to, legalRoads[0].length};
        AddMoveToCityandTotal(agent, legalRoads[0].to);
        free(roads);
        free(legalRoads);
        return move;
    }

    // If agent hasn't made any moves yet to cities other than agent the city that agent started at
    if (agent->totalMoves == 1) {
        Road *leastLengthArray = malloc(MapNumCities(m) * sizeof(Road));
        int numMinLengthCities = minLength(m, legalRoads, numLegalRoads, leastLengthArray);    
        assert(numMinLengthCities > 0);
        move = (Move){leastLengthArray[0].to, leastLengthArray[0].length};
        AddMoveToCityandTotal(agent, leastLengthArray[0].to);
        free(roads);
        free(leastLengthArray);
        free(legalRoads);
        return move;
    } else {
        Road *leastLengthArray = malloc(MapNumCities(m) * sizeof(Road));
        Road *leastMovesArray = malloc(MapNumCities(m) * sizeof(Road));
        int numMinMovesCities = minMoves(agent, legalRoads, numLegalRoads, leastMovesArray);
        int numMinLengthCities = minLength(m, leastMovesArray, numMinMovesCities, leastLengthArray);
        assert(numMinLengthCities > 0);
        move = (Move){leastLengthArray[0].to, leastLengthArray[0].length};
        AddMoveToCityandTotal(agent, leastLengthArray[0].to);
        free(roads);
        free(leastLengthArray);
        free(legalRoads);
        free(leastMovesArray);
        return move;
    }
}

// finds cities that have been visited the least number of times and adjacent to the current location of the agent
int minMoves(Agent agent, Road legalRoads[], int numLegalRoads, Road leastMovesArray[]) {
    int minMoves = agent->moves[legalRoads[0].to];
    int num = 0;
    for (int i = 0; i < numLegalRoads; i++) {
        if (agent->moves[legalRoads[i].to] < minMoves) {
            minMoves = agent->moves[legalRoads[i].to];
        }
    }   

    for (int i = 0; i < numLegalRoads; i++) {
        if (agent->moves[legalRoads[i].to] == minMoves) {
            leastMovesArray[num] = legalRoads[i];
            num++;
        }
    }
    return num;
}

// finds the cities that will take least stamina for the agent to travel to.
int minLength(Map m, Road legalRoads[], int numLegalRoads, Road leastLengthArray[]) {
    int minLength = legalRoads[0].length;
    int num = 0;
    for (int i = 0; i < numLegalRoads; i++) {
        if (legalRoads[i].length < minLength) {
            minLength = legalRoads[i].length;
        }
    }   

    for (int i = 0; i < numLegalRoads; i++) {
        if (legalRoads[i].length == minLength) {
            leastLengthArray[num] = legalRoads[i];
            num++;
        }
    }
    return num;
}

// Adds the number of moves to the city to count how many times it has been visited
void AddMoveToCityandTotal(Agent agent, int city) {
    agent->moves[city]++;
    agent->totalMoves++;
}

// This function allows the agents to pick a city randomly out of the adjacent ones
static Move chooseRandomMove(Agent agent, Map m) {
    agent->strategy = RANDOM;
    agent->initialStrategy = RANDOM;
    Road *roads = malloc(MapNumCities(m) * sizeof(Road));
    Road *legalRoads = malloc(MapNumCities(m) * sizeof(Road));

    // Get all roads to adjacent cities
    int numRoads = MapGetRoadsFrom(m, agent->location, roads);

    // Filter out roads that the agent does not have enough stamina for
    int numLegalRoads = filterRoads(agent, roads, numRoads, legalRoads);

    Move move;
    if (numLegalRoads > 0) {
        // Sort the roads using insertion sort
        for (int i = 1; i < numLegalRoads; i++) {
            Road r = legalRoads[i];
            int j = i;
            while (j > 0 && r.to < legalRoads[j - 1].to) {
                legalRoads[j] = legalRoads[j - 1];
                j--;
            }
            legalRoads[j] = r;
        }
        
        // nextMove is randomly chosen from the legal roads
        int k = rand() % numLegalRoads;
        move = (Move){legalRoads[k].to, legalRoads[k].length};
    } else {
        // The agent must stay in the same location
        move = (Move){agent->location, 0};
    }
    
    free(legalRoads);
    free(roads);
    return move;
}

// Takes an array with all the possible roads and puts the ones the agent
// has enough stamina for into the legalRoads array
// Returns the number of legal roads
static int filterRoads(Agent agent, Road roads[], int numRoads,
                       Road legalRoads[]) {
    int numLegalRoads = 0;
    for (int i = 0; i < numRoads; i++) {
        if (roads[i].length <= agent->stamina) {
            legalRoads[numLegalRoads++] = roads[i];
        }
    }
    return numLegalRoads;
}

/**
 * Executes a given move by updating the agent's internal state
 */
void AgentMakeNextMove(Agent agent, Move move) {
    if (move.to == agent->location) {
        agent->stamina = agent->maxStamina;
    } else {
        agent->stamina -= move.staminaCost;
    }
    agent->location = move.to;
    
    // TODO: You may need to add to this to handle different strategies
}

////////////////////////////////////////////////////////////////////////
// Gets information about the agent
// NOTE: It is expected that these functions do not need to be modified

/**
 * Gets the name of the agent
 */
char *AgentName(Agent agent) {
    return agent->name;
}

/**
 * Gets the current location of the agent
 */
int AgentLocation(Agent agent) {
    return agent->location;
}

/**
 * Gets the amount of stamina the agent currently has
 */
int AgentStamina(Agent agent) {
    return agent->stamina;
}

////////////////////////////////////////////////////////////////////////
// Learning information

/**
 * Tells the agent where the thief is and the new strategy that the agent has to follow
 */
void AgentGainInfo(Agent agent, int thiefLocation) {
    // This is to set thiefs current location
    agent->givenThiefLocation = thiefLocation;

    // This is initializing the new strategy 
    agent->strategy = LEAST_TURNS_PATH;
}

////////////////////////////////////////////////////////////////////////
// Displaying state

/**
 * Prints information about the agent (for debugging purposes)
 */
void AgentShow(Agent agent) {
    // TODO: You can implement this function however you want
    //       You can leave this function blank if you want
}

////////////////////////////////////////////////////////////////////////