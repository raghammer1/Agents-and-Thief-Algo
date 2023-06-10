// // Implementation of the Map ADT

// #include <assert.h>
// #include <stdbool.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>

// #include "Map.h"

// typedef struct names{
//     int cityNum;
//     char *cityName;
// } names;

// struct map{
//     // TODO
//     int numCities;
//     int numRoads;
//     Road *roads;
//     names *names;
//     // struct Graph *graph;
//     // int num_cities;
// };

// // typedef struct names names;

// // struct Graph {
// //     Road *road;
// //     struct Graph* next;
// // };

// bool checkIfRoadAlreadyExists(Map m, int city1, int city2, int length);

// /**
//  * Creates a new map with the given number of cities
//  * Assumes that numCities is positive
//  */
// Map MapNew(int numCities) {
//     // WORKING
//     assert(numCities > 0);
//     Map m = malloc(sizeof(Map));
//     if (m == NULL) {
//         fprintf(stderr, "error: out of memory\n");
//         exit(EXIT_FAILURE);
//     }
//     m->numCities = numCities;
//     m->numRoads = 0;
//     m->roads = malloc(numCities * sizeof(Road));
//     m->names = malloc(numCities * sizeof(names));
//     if (m->roads == NULL) {
//         fprintf(stderr, "error: out of memory\n");
//         exit(EXIT_FAILURE);
//     }
//     for (int i = 0; i < numCities; i++) {
//         m->names[i].cityNum = i;
//         // if (m->roads[i] == NULL) {
//         //     fprintf(stderr, "error: out of memory\n");
//         //     exit(EXIT_FAILURE);
//         // }
//         // if (m->names[i] == NULL) {
//         //     fprintf(stderr, "error: out of memory\n");
//         //     exit(EXIT_FAILURE);
//         // }
//     }
//     return m;
// }

// /**
//  * Frees all memory associated with the given map
//  */
// void MapFree(Map m) {
//     // WORKING
//     free(m->names);
//     free(m->roads);
//     free(m);
// }

// /**
//  * Returns the number of cities on the given map
//  */
// int MapNumCities(Map m) {
//     return m->numCities;
// }

// /**
//  * Returns the number of roads on the given map
//  */
// int MapNumRoads(Map m) {
//     return m->numRoads;
// }

// /**
//  * Inserts a road between two cities with the given length
//  * Does nothing if there is already a road between the two cities
//  * Assumes that the cities are valid and are not the same
//  * Assumes that the length of the road is positive
//  */
// void MapInsertRoad(Map m, int city1, int city2, int length) {
//     // Working
//     bool checker = checkIfRoadAlreadyExists(m, city1, city2, length);
//     if (checker == true) {
//         return;
//     }
//     assert(length > 0);
//     assert(city1 != city2);

//     for (int i = 0; i < m->numCities; i++) {
//         if (m->roads[i].to == 0 && m->roads[i].from == 0 && m->roads[i].length == 0) {
//             m->roads[i].to = city1;
//             m->roads[i].from = city2;
//             m->roads[i].length = length;
//             m->numRoads++;
//             return;
//         }
//     }
// }

// bool checkIfRoadAlreadyExists(Map m, int city1, int city2, int length) {
//     // Working
//     for (int i = 0; i < m->numRoads; i++) {
//         if (m->roads[i].to == city1 && m->roads[i].from == city2) {
//             return true;
//         }
//         if (m->roads[i].to == city2 && m->roads[i].from == city1) {
//             return true;
//         }
//     }
//     return false;
// }

// /**
//  * Sets the name of the given city
//  */
// void MapSetName(Map m, int city, char *name) {
//     // WORKING
//     // for (int i = 0; i < m->numCities; i++) {
//     //     if (m->names[i]->cityNum == city) {
//     //         return;
//     //     }
//     // }
//     // for (int i = 0; i < m->numCities; i++) {
//     //     if (m->names[i]->cityNum == city) {
//     //         m->names[i]->cityName = name;
//     //         return;
//     //     }
//     // }
//     m->names[city].cityName = name;
//     // TODO
// }

// /**
//  * Returns the name of the given city
//  */
// char *MapGetName(Map m, int city) {
//     // WORKING
//     // for (int i = 0; i < m->numCities; i++) {
//     //     if (m->names[i]->cityNum == city) {
//     //         return m->names[i]->cityName;
//     //     }
//     // }
//     return m->names[city].cityName;
//     fprintf(stderr, "Incorrect city num given\n");
//     return("City does not exist");
// }

// /**
//  * Checks if there is a road between the two given cities
//  * Returns the length of the road if there is a road, and 0 otherwise
//  */
// int MapContainsRoad(Map m, int city1, int city2) {
//     // Working
//     for (int i = 0; i < m->numRoads; i++) {
//         if (m->roads[i].to == city1 && m->roads[i].from == city2) {
//             return m->roads[i].length;
//         }
//         if (m->roads[i].from == city1 && m->roads[i].to == city2) {
//             return m->roads[i].length;
//         }
//     }
//     fprintf(stderr, "No road exists between the two cities\n");
//     return 0;
// }

// /**
//  * Returns the number of roads connected to the given city and stores
//  * them in the given roads array. The `from` field should be equal to
//  * `city` for all the roads in the array.
//  * Assumes that the roads array is large enough to store all the roads
//  */
// int MapGetRoadsFrom(Map m, int city, Road roads[]) {
//     // int roadsConnected = 0;
//     // for (int i = 0; i < m->numRoads*2; i++) {
//     //     if (m->roads[i] == NULL) {
//     //         return roadsConnected;
//     //     }
//     //     if (m->roads[i]->from == city) {
//     //         roadsConnected++;
//     //     }
//     // }
//     // // TODO Complete this function
//     // return 0;
//     int roadsConnected = 0;
//     for (int i = 0; i < m->numRoads; i++) {
//     //     if (m->roads[i] == NULL) {
//     //         return roadsConnected;
//     //     }
//         if (m->roads[i].from == city) {
//             roads[roadsConnected].from = m->roads[i].from;
//             roads[roadsConnected].to = m->roads[i].to;
//             roads[roadsConnected].length = m->roads[i].length;
//             roadsConnected++;
//         }
//     }
//     // TODO Complete this function
//     return roadsConnected;
// }

// /**
//  * Displays the map
//  * !!! DO NOT EDIT THIS FUNCTION !!!
//  * This function will work once the other functions are working
//  */
// void MapShow(Map m) {
//     printf("Number of cities: %d\n", MapNumCities(m));
//     printf("Number of roads: %d\n", MapNumRoads(m));

//     Road *roads = malloc(MapNumRoads(m) * sizeof(Road));
//     if (roads == NULL) {
//         fprintf(stderr, "error: out of memory\n");
//         exit(EXIT_FAILURE);
//     }

//     for (int i = 0; i < MapNumCities(m); i++) {
//         printf("[%d] %s has roads to:", i, MapGetName(m, i));
//         int numRoads = MapGetRoadsFrom(m, i, roads);
//         for (int j = 0; j < numRoads; j++) {
//             if (j > 0) {
//                 printf(",");
//             }
//             printf(" [%d] %s (%d)", roads[j].to, MapGetName(m, roads[j].to),
//                    roads[j].length);
//         }
//         printf("\n");
//     }

//     free(roads);
// }

// int main(void) {
//     Map m = MapNew(10);
//     // int h = int (sizeof(m->names));
//     printf("Citu2Num is %d\n", m->names[2].cityNum);
//     for (int i = 0; i < 10; i++) {
//         MapSetName(m, i, "Syna");
//     }
//     for (int i = 0; i < 10; i++) {
//         printf("%s\n", MapGetName(m, i));
//     }
//     printf("Citu2Num is %s\n", m->names[5].cityName);
//     printf("Citu2Num is %s\n", MapGetName(m, 2));
//     printf("HERE\n");
//     MapInsertRoad(m, 2, 1, 100);
//     printf("HERE\n");
//     MapInsertRoad(m, 2, 1, 1000);
//     printf("HERE\n");
//     printf("%d\n",MapContainsRoad(m, 2,1));
//     printf("%d",MapNumRoads(m));
//     printf("Number of cities: %d\n", MapNumCities(m));
//     printf("Number of roads: %d\n", MapNumRoads(m));
//     MapInsertRoad(m, 3, 1, 100);
//     MapInsertRoad(m, 4, 1, 100);
//     MapInsertRoad(m, 5, 1, 100);
//     MapInsertRoad(m, 6, 1, 100);
//     MapInsertRoad(m, 2, 5, 100);
//     MapInsertRoad(m, 2, 6, 100);
//     MapInsertRoad(m, 2, 8, 100);
//     MapInsertRoad(m, 2, 4, 100);
//     MapInsertRoad(m, 2, 3, 100);
//     Road *road = malloc(10*sizeof(Road));
//     printf("WOOOOOW OWOOOWO %d\n", MapGetRoadsFrom(m,1,road));
//     Road *roads = malloc(MapNumRoads(m) * sizeof(Road));
//     if (roads == NULL) {
//         fprintf(stderr, "error: out of memory\n");
//         exit(EXIT_FAILURE);
//     }

//     for (int i = 0; i < MapNumCities(m); i++) {
//         printf("[%d] %s has roads to:", i, MapGetName(m, i));
//         int numRoads = MapGetRoadsFrom(m, i, roads);
//         for (int j = 0; j < numRoads; j++) {
//             if (j > 0) {
//                 printf(",");
//             }
//             printf(" [%d] %s (%d)", roads[j].to, MapGetName(m, roads[j].to),
//                    roads[j].length);
//         }
//         printf("\n");
//     }
//     return 0;
// }

// Implementation of the Map ADT

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Map.h"

typedef struct names
{
    int cityNum;
    char *cityName;
} names;

struct map
{
    // TODO
    int numCities;
    int numRoads;
    Road *roads;
    names *names;
    int maxNumRoads;
    // struct Graph *graph;
    // int num_cities;
};

// typedef struct names names;

// struct Graph {
//     Road *road;
//     struct Graph* next;
// };

bool checkIfRoadAlreadyExists(Map m, int city1, int city2, int length);
Road *AllRoads(Map m);

/**
 * Creates a new map with the given number of cities
 * Assumes that numCities is positive
 */

Map MapNew(int numCities)
{
    // WORKING
    assert(numCities > 0);
    Map m = malloc(sizeof(Map));
    if (m == NULL)
    {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }
    m->numCities = numCities;
    m->numRoads = 0;
    int MaxNumEdges = (numCities - 1);
    MaxNumEdges = ((2 * numCities * MaxNumEdges) / 2);
    // printf("MaxNumEdges: %d\n", MaxNumEdges);
    m->roads = malloc(MaxNumEdges * sizeof(Road));
    m->names = malloc(numCities * sizeof(names));
    m->maxNumRoads = MaxNumEdges;
    if (m->roads == NULL)
    {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < numCities; i++)
    {
        m->names[i].cityNum = i;
        // if (m->roads[i] == NULL) {
        //     fprintf(stderr, "error: out of memory\n");
        //     exit(EXIT_FAILURE);
        // }
        // if (m->names[i] == NULL) {
        //     fprintf(stderr, "error: out of memory\n");
        //     exit(EXIT_FAILURE);
        // }
    }
    return m;
}

/**
 * Frees all memory associated with the given map
 */
void MapFree(Map m)
{
    // WORKING
    free(m->names);
    free(m->roads);
    free(m);
}

/**
 * Returns the number of cities on the given map
 */
int MapNumCities(Map m)
{
    return m->numCities;
}

/**
 * Returns the number of roads on the given map
 */
int MapNumRoads(Map m)
{
    return m->numRoads;
}

/**
 * Inserts a road between two cities with the given length
 * Does nothing if there is already a road between the two cities
 * Assumes that the cities are valid and are not the same
 * Assumes that the length of the road is positive
 */
void MapInsertRoad(Map m, int city1, int city2, int length)
{
    // Working
    bool checker = checkIfRoadAlreadyExists(m, city1, city2, length);
    if (checker == true)
    {
        return;
    }
    assert(length > 0);
    assert(city1 != city2);
    int i = 0;
    while (i < m->maxNumRoads)
    {

        if (m->roads[i].to == 0 && m->roads[i].from == 0 && m->roads[i].length == 0)
        {
            m->roads[i].to = city1;
            m->roads[i].from = city2;
            m->roads[i].length = length;
            m->roads[i + 1].from = city1;
            m->roads[i + 1].to = city2;
            m->roads[i + 1].length = length;
            m->numRoads++;
            ;
            return;
        }
        i++;
    }
    // i = 0;
    // while (i < m->maxNumRoads) {
    //     if (m->roads[i].to == 0 && m->roads[i].from == 0 && m->roads[i].length == 0) {
    //         m->roads[i].from = city1;
    //         m->roads[i].to = city2;
    //         m->roads[i].length = length;
    //         // m->numRoads++;
    //         return;
    //     }
    //     i++;
    // }
}

bool checkIfRoadAlreadyExists(Map m, int city1, int city2, int length)
{
    // Working
    for (int i = 0; i < m->numCities; i++)
    {
        if (m->roads[i].to == city1 && m->roads[i].from == city2)
        {
            return true;
        }
        if (m->roads[i].to == city2 && m->roads[i].from == city1)
        {
            return true;
        }
    }
    return false;
}

/**
 * Sets the name of the given city
 */
void MapSetName(Map m, int city, char *name)
{
    // WORKING
    // for (int i = 0; i < m->numCities; i++) {
    //     if (m->names[i]->cityNum == city) {
    //         return;
    //     }
    // }
    // for (int i = 0; i < m->numCities; i++) {
    //     if (m->names[i]->cityNum == city) {
    //         m->names[i]->cityName = name;
    //         return;
    //     }
    // }
    m->names[city].cityName = name;
    // TODO
}

/**
 * Returns the name of the given city
 */
char *MapGetName(Map m, int city)
{
    // WORKING
    // for (int i = 0; i < m->numCities; i++) {
    //     if (m->names[i]->cityNum == city) {
    //         return m->names[i]->cityName;
    //     }
    // }
    return m->names[city].cityName;
    fprintf(stderr, "Incorrect city num given\n");
    return ("City does not exist");
}

Road *AllRoads(Map m)
{
    return m->roads;
}

/**
 * Checks if there is a road between the two given cities
 * Returns the length of the road if there is a road, and 0 otherwise
 */
int MapContainsRoad(Map m, int city1, int city2)
{
    // Working
    for (int i = 0; i < m->numCities; i++)
    {
        if (m->roads[i].to == city1 && m->roads[i].from == city2)
        {
            return m->roads[i].length;
        }
        if (m->roads[i].from == city1 && m->roads[i].to == city2)
        {
            return m->roads[i].length;
        }
    }
    fprintf(stderr, "No road exists between the two cities\n");
    return 0;
}

/**
 * Returns the number of roads connected to the given city and stores
 * them in the given roads array. The `from` field should be equal to
 * `city` for all the roads in the array.
 * Assumes that the roads array is large enough to store all the roads
 */
int MapGetRoadsFrom(Map m, int city, Road roads[])
{
    // int roadsConnected = 0;
    // for (int i = 0; i < m->numRoads*2; i++) {
    //     if (m->roads[i] == NULL) {
    //         return roadsConnected;
    //     }
    //     if (m->roads[i]->from == city) {
    //         roadsConnected++;
    //     }
    // }
    // // TODO Complete this function
    // return 0;
    int roadsConnected = 0;
    for (int i = 0; i < m->numRoads * 2; i++)
    {
        //     if (m->roads[i] == NULL) {
        //         return roadsConnected;
        //     }
        if (m->roads[i].from == city)
        {
            roads[roadsConnected] = m->roads[i];
            roadsConnected++;
        }
    }
    // TODO Complete this function
    return roadsConnected;
}

/**
 * Displays the map
 * !!! DO NOT EDIT THIS FUNCTION !!!
 * This function will work once the other functions are working
 */
void MapShow(Map m)
{
    printf("Number of cities: %d\n", MapNumCities(m));
    printf("Number of roads: %d\n", MapNumRoads(m));

    Road *roads = malloc(MapNumRoads(m) * sizeof(Road));
    if (roads == NULL)
    {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < MapNumCities(m); i++)
    {
        printf("[%d] %s has roads to:", i, MapGetName(m, i));
        int numRoads = MapGetRoadsFrom(m, i, roads);
        for (int j = 0; j < numRoads; j++)
        {
            if (j > 0)
            {
                printf(",");
            }
            printf(" [%d] %s (%d)", roads[j].to, MapGetName(m, roads[j].to),
                   roads[j].length);
        }
        printf("\n");
    }

    free(roads);
}
