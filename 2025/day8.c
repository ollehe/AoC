#include <stdint.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static const uint64_t MAX_LINES = 4096;
static const uint64_t MAX_LINE_LENGTH = 4096;

typedef uint64_t JunctionBoxIndex;
typedef uint64_t CircuitId;

// Coordinate (also vertex)
typedef struct {
	long long x;
	long long y;
	long long z;
} JunctionBox;

// Undirected edge
typedef struct {
	JunctionBoxIndex a; // Corresponds to the index of the first junction box
	JunctionBoxIndex b; // Corresponds to the index of the second junction box
	uint64_t weight;
} Edge;

typedef struct {
	CircuitId id;
	JunctionBoxIndex *boxIds;
	uint64_t size;
	uint64_t capacity;
} Circuit;

static int compareEdges(const void *a, const void *b)
{
	const Edge edgeA = *(const Edge *)a;
	const Edge edgeB = *(const Edge *)b;
	if (edgeA.weight < edgeB.weight)
		return -1;
	else if (edgeA.weight > edgeB.weight)
		return 1;
	else
		return 0;
}

static uint64_t readFile(const char *fileName, char **lines)
{
	FILE *file = fopen(fileName, "r");
	if (file == NULL) {
		perror("Error opening file");
		exit(EXIT_FAILURE);
	}
	char buffer[MAX_LINE_LENGTH];
	uint64_t count = 0;
	while (fgets(buffer, sizeof(buffer), file) != NULL &&
	       count < MAX_LINES) {
		lines[count] = malloc(strlen(buffer) +
				      1); // Allocate memory for each line
		if (lines[count] == NULL) {
			perror("Error allocating memory");
			fclose(file);
			exit(EXIT_FAILURE);
		}

		strcpy(lines[count], buffer); // Copy the line into the array

		uint64_t len = strlen(lines[count]);
		if (len > 0 && lines[count][len - 1] == '\n') {
			lines[count][len - 1] =
				'\0'; // Replace newline with null terminator
		}
		count++;
	}
	fclose(file);
	return count; // Return the number of lines read
}

static void parseInput(char **data, uint64_t rows, JunctionBox **boxes)
{
	*boxes = malloc(rows * sizeof(JunctionBox));
	if (*boxes == NULL) {
		perror("Error allocating memory");
		exit(EXIT_FAILURE);
	}
	for (uint64_t row = 0; row < rows; row++) {
		if (sscanf(data[row], "%lld,%lld,%lld", &(*boxes)[row].x,
			   &(*boxes)[row].y, &(*boxes)[row].z) != 3) {
			perror("Error parsing input");
			exit(EXIT_FAILURE);
		}
	}
}

static void printJunctionBoxes(JunctionBox *box, uint64_t rows)
{
	for (uint64_t i = 0; i < rows; i++) {
		printf("Junctionbox %zu: (%lld, %lld, %lld)\n", i, box[i].x,
		       box[i].y, box[i].z);
	}
}

static void printEdges(Edge *edges, uint64_t numberOfEdges)
{
	for (uint64_t i = 0; i < numberOfEdges; i++) {
		printf("Edge %zu: (%zu, %zu) weight %zu\n", i, edges[i].a,
		       edges[i].b, edges[i].weight);
	}
}
static void printCircuit(Circuit *circuit)
{
	printf("Circuit %zu. Size %zu\n", circuit->id, circuit->size);
	for (uint64_t i = 0; i < circuit->size; i++) {
		printf("Box Id: %zu\n", circuit->boxIds[i]);
	}
}

static void printGraph(JunctionBox *coordinates, uint64_t rows, Edge *edges,
		       uint64_t numberOfEdges)
{
	printJunctionBoxes(coordinates, rows);
	printEdges(edges, numberOfEdges);
}

static uint64_t squaredEuclideanDistance(const JunctionBox *u,
					 const JunctionBox *w)
{
	long long dx = (u->x - w->x);
	long long dy = (u->y - w->y);
	long long dz = (u->z - w->z);
	return (uint64_t)(dx * dx + dy * dy + dz * dz);
}

static void computeEdges(JunctionBox *coordinates, uint64_t rows, Edge **edges,
			 uint64_t *numberOfEdges)
{
	// The graph is complete so allocate rows*(rows-1)/2 many edges.
	*numberOfEdges = rows * (rows - 1) / 2;
	*edges = malloc(*numberOfEdges * sizeof(Edge));
	if (*edges == NULL) {
		perror("Error allocating memory");
		exit(EXIT_FAILURE);
	}
	uint64_t index = 0;
	for (JunctionBoxIndex i = 0; i < rows; i++) {
		for (JunctionBoxIndex j = i + 1; j < rows; j++) {
			uint64_t weight = squaredEuclideanDistance(
				&coordinates[i], &coordinates[j]);
			Edge ij = { .a = i, .b = j, .weight = weight };
			(*edges)[index++] = ij;
		}
	}
	// Sort edges by weight ascending.
	qsort(*edges, *numberOfEdges, sizeof(Edge), compareEdges);
}

static bool intersects(Circuit *c, Edge *e)
{
	// Check if one id is contained in the circuit
	bool a = false;
	bool b = false;
	for (uint64_t i = 0; i < c->size; i++) {
		if (c->boxIds[i] == e->a) {
			a = true;
		}
		if (c->boxIds[i] == e->b) {
			b = true;
		}
	}
	return a || b;
}

static bool included(Circuit *c, Edge *e)
{
	// Check if both ids are contained in the circuit
	bool a = false;
	bool b = false;
	for (uint64_t i = 0; i < c->size; i++) {
		if (c->boxIds[i] == e->a) {
			a = true;
		}
		if (c->boxIds[i] == e->b) {
			b = true;
		}
	}
	return a && b;
}

static void addEdge(Circuit *c, Edge *e)
{
	// If size is 0 add both (which basically creates a new active circuit)
	if (c->size == 0) {
		c->boxIds[c->size++] = e->a;
		c->boxIds[c->size++] = e->b;
		return;
	}
	// Otherwise check which is added
	bool added = false;
	// First check if a is added
	for (uint64_t i = 0; i < c->size; i++) {
		if (c->boxIds[i] == e->a) {
			added = true;
			break;
		}
	}
	if (!added) {
		c->boxIds[c->size++] = e->a;
	}
	// Then check if b is added
	added = false;
	for (uint64_t i = 0; i < c->size; i++) {
		if (c->boxIds[i] == e->b) {
			added = true;
			break;
		}
	}
	if (!added) {
		c->boxIds[c->size++] = e->b;
	}
}

static bool isBoxIdPresent(Circuit *c, uint64_t id)
{
	for (uint64_t i = 0; i < c->size; i++) {
		if (c->boxIds[i] == id) {
			return true;
		}
	}
	return false;
}

static void mergeCircuits(Circuit *circuits, uint64_t *numberOfCircuits,
			  uint64_t indices[2])
{
	// Move everything to the circuit with the smaller index
	uint64_t smallerIndex = indices[0] < indices[1] ? indices[0] :
							  indices[1];
	uint64_t largerIndex = indices[0] < indices[1] ? indices[1] :
							 indices[0];
	for (uint64_t i = 0; i < circuits[largerIndex].size; i++) {
		//Add only ids that are not already present
		if (!isBoxIdPresent(&circuits[smallerIndex],
				    circuits[largerIndex].boxIds[i])) {
			circuits[smallerIndex]
				.boxIds[circuits[smallerIndex].size++] =
				circuits[largerIndex].boxIds[i];
		}
	}
	free(circuits[largerIndex].boxIds);
	circuits[largerIndex].boxIds = NULL;
	circuits[largerIndex].size = 0;

	// Shift the remaining circuits to the left to fill the gap
	for (uint64_t i = largerIndex; i < *numberOfCircuits - 1; i++) {
		circuits[i] = circuits[i + 1];
		circuits[i + 1].boxIds = NULL;
		circuits[i + 1].size = 0;
	}
	(*numberOfCircuits)--;
	// Free the memory of the merged circuit
}

static Circuit *allocateCircuits(uint64_t numberOfCircuits,
				 uint64_t numberOfIds)
{
	Circuit *circuits = malloc(numberOfCircuits * sizeof(Circuit));
	if (circuits == NULL) {
		perror("Error allocating memory");
		exit(EXIT_FAILURE);
	}
	// For each circuit allocate 2*numberOfConnections junction boxes
	// since all edges might consist of unique pairs of junction boxes
	for (uint64_t i = 0; i < numberOfCircuits; i++) {
		circuits[i].id = i;
		circuits[i].size = 0;
		circuits[i].boxIds =
			malloc(numberOfIds * sizeof(JunctionBoxIndex));
		if (circuits[i].boxIds == NULL) {
			perror("Error allocating memory");
			exit(EXIT_FAILURE);
		}
	}
	return circuits;
}

static void connectJunctionBoxes(uint64_t numberOfConnections, Edge *edges,
				 uint64_t numberOfEdges,
				 JunctionBox *junctionBoxes,
				 uint64_t numberOfJunctionBoxes)
{
	if (numberOfConnections >= numberOfEdges) {
		return;
	}
	uint64_t totalNumberOfCircuits = numberOfConnections + 1;
	Circuit *circuits = allocateCircuits(totalNumberOfCircuits,
					     2 * numberOfConnections + 1);

	uint64_t numberOfActiveCircuits = 0;
	bool added = false;
	// This keeps track of the indices of the
	uint64_t addedCircuitIndices[2] = { 0, 0 };
	uint8_t index = 0;
	for (uint64_t i = 0; i < numberOfConnections; i++) {
		// Check if the edge is already contained in an active circuit
		// or intersects a circuit.
		// If it intersects, add it to the circuit (might be added to another circuit!)
		added = false;
		index = 0;
		for (uint64_t j = 0; j < numberOfActiveCircuits; j++) {
			if (included(&circuits[j], &edges[i])) {
				printf("Edge %zu is already contained in circuit %zu\n",
				       i, j);
				added = true;
				break;
			} else if (intersects(&circuits[j], &edges[i])) {
				addEdge(&circuits[j], &edges[i]);
				added = true;
				// An edge can only be part of at most two circuits
				addedCircuitIndices[index++] = j;
				printf("Index %d\n", index);
			}
		}
		// If neither happens, we "create" a new active circuit by adding it to the last circuit.
		if (!added) {
			addEdge(&circuits[numberOfActiveCircuits], &edges[i]);
			numberOfActiveCircuits++;
		} else if (index == 2) {
			// If the edge is part of two circuits, merge them
			printf("Merging circuits %zu and %zu\n",
			       addedCircuitIndices[0], addedCircuitIndices[1]);
			mergeCircuits(circuits, &numberOfActiveCircuits,
				      addedCircuitIndices);
		}
	}

	// Free circuits
	for (uint64_t i = 0; i < totalNumberOfCircuits; i++) {
		printCircuit(&circuits[i]);
		free(circuits[i].boxIds);
		circuits[i].boxIds = NULL;
		circuits[i].size = 0;
	}
	free(circuits);
}

int main(void)
{
	char *data[MAX_LINES];
	// Read file
	uint64_t rows = readFile("input.txt", data);
	// Parse data
	JunctionBox *boxes;
	parseInput(data, rows, &boxes);
	// Free memory
	for (uint64_t i = 0; i < rows; i++) {
		free(data[i]);
	}
	// Compute edges
	Edge *edges;
	uint64_t numberOfEdges;
	computeEdges(boxes, rows, &edges, &numberOfEdges);
	// Form connections
	connectJunctionBoxes(1000, edges, numberOfEdges, boxes, rows);
	// Free memory
	free(boxes);
	free(edges);
	return 0;
}
