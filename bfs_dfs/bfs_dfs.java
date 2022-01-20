public void BFS(Node s) {
  System.out.print("\nBFS\n");

  // Create Boolean ArrayList of size = to number of nodes in Graph object
  int n = this.nodes.size();
  ArrayList<Boolean> discovered = new ArrayList<Boolean>(n);

  // Initialize all Boolean values to false then update starting node to true
  for (int i = 0; i < n; ++i) {
    discovered.add(false);
  }
  // Set starting node as both discovered and then also explored and print first explore statement
  discovered.set(s.name-1, true);

  // Create ArrayList of Queues - where each Queue is a layer - add starting layer to it
  ArrayList<Queue<Node>> layers = new ArrayList<Queue<Node>>();

  LinkedList<Node> initLayer = new LinkedList<Node>();
  layers.add(initLayer);

  //Initialize L[0] to consist of the single element s
  initLayer.add(s);

  //Set the layer counter i = 0
  int layerCount = 0;

  //While L[i] is not empty
  while(!layers.get(layerCount).isEmpty()) {
      // Initialize an empty list L[i + 1] -- create layer
      LinkedList<Node> nextLayer = new LinkedList<Node>();
      // Print out the node being explored
      Node currNode;

      // For each node u ∈ L[i] -- goes through nodes in current layer
      while(!layers.get(layerCount).isEmpty()) {
          currNode = layers.get(layerCount).element();

          System.out.printf("explore: %d\n", currNode.name);

          // Goes through adjlist of each node in current layer (only loops num of nodes in adjlist)
          for (int i = 0; i < currNode.adjlist.size(); ++i) {
              //If Discovered[v] = false then
              if (!discovered.get(currNode.adjlist.get(i).name - 1)) {
                  //Set Discovered[v] = true
                  discovered.set(currNode.adjlist.get(i).name - 1, true);
                  if (layerCount % 2 == 0) {
                      currNode.adjlist.get(i).marked = true;
                  }

                  // Print out the node you've discovered in the adjlist
                  System.out.printf("tree: %d - %d\n", currNode.name,
                          currNode.adjlist.get(i).name);
                  if (currNode.adjlist.get(i).marked) {
                      System.out.printf("discover: %d (marked)\n", currNode.adjlist.get(i).name);
                  } else {
                      System.out.printf("discover: %d (unmarked)\n", currNode.adjlist.get(i).name);
                  }

                  // Adds the node in adjlist to the nextLayer for future exploration
                  nextLayer.add(currNode.adjlist.get(i));
              }
          }
          // Pops off the front of the queue once done exploring/discovering with it
          layers.get(layerCount).poll();
      }
      // Add the nextLayer we've gotten from the adjlists and increase layer count
      layers.add(nextLayer);
      ++layerCount;
  }

  checkForOddCycle();
}

// this assumes the nodes have already been marked, from a BFS traversal
public void checkForOddCycle() {
  boolean hasOddCycle = false;

  ArrayList<Queue<Node>> layers = new ArrayList<Queue<Node>>();
  LinkedList<Node> currLayer = new LinkedList<Node>();

  currLayer.add(this.nodes.get(0));
  layers.add(currLayer);

  int n = this.nodes.size();
  ArrayList<Boolean> discovered = new ArrayList<Boolean>(n);

  // Initialize all Boolean values to false then update starting node to true
  for (int i = 0; i < n; ++i) {
      discovered.add(false);
  }
  // Set starting node as both discovered and then also explored and print first explore statement
  discovered.set(currLayer.get(0).name-1, true);

  //Set the layer counter i = 0
  int layerCount = 0;

  //While L[i] is not empty // last loop will be the final empty layer
  while(!layers.get(layerCount).isEmpty()) {
      LinkedList<Node> nextLayer = new LinkedList<Node>();
      currLayer = (LinkedList<Node>) layers.get(layerCount);
      for (int i = 0; i < currLayer.size(); ++i) {
          Node currNode = currLayer.get(i);

          for (int j = 0; j < currNode.adjlist.size(); ++j) {
              if (!discovered.get(currNode.adjlist.get(j).name - 1)) {
                  // only add if not previously discovered
                  discovered.set(currNode.adjlist.get(j).name - 1, true);
                  nextLayer.add(currNode.adjlist.get(j));
              }
          }
          // Loops through currLayer and checks if any of the nodes have the currNode in their adjList
          for (int j = 0; j < currLayer.size(); ++j) {
              if (currLayer.get(j).adjlist.contains(currNode)) {
                  System.out.printf("%d - %d: both nodes same color\n", currNode.name,
                          currLayer.get(j).name);
                  hasOddCycle = true;
              }
          }
      }
      layers.add(nextLayer);
      ++layerCount;
  }

  if (hasOddCycle) {
      System.out.print("Graph has at least one odd cycle");
  }
}

public void DFS(Node s) {
  System.out.print("DFS\n");

  //Initialize S to be a stack with one element s
  Stack<Node> stack = new Stack<Node>();
  stack.add(s);

  // Create Boolean ArrayList of size = to number of nodes in Graph object
  int n = this.nodes.size();
  ArrayList<Boolean> explored = new ArrayList<Boolean>(n);

  // Initialize all Boolean values to false then update starting node to true
  for (int i = 0; i < n; ++i) {
      explored.add(false);
  }

  // Parent ArrayList - set entire list to null to start
  ArrayList<Node> parent = new ArrayList<Node>(n);
  for (int i = 0; i < n; ++i) {
      parent.add(null);
  }

  //While S is not empty
  while(!stack.isEmpty()) {
      //Take a node u from S
      Node currNode = stack.pop();

      //If Explored[u] = false then
      if (!explored.get(currNode.name - 1)) {
          //Set Explored[u] = true
          explored.set(currNode.name - 1, true);
          System.out.printf("explore: %d\n", currNode.name);
          if (parent.get(currNode.name-1) != null) { // if the parent node for the currNode isn't set we don't wish to print anything
              System.out.printf("tree: %d - %d\n", currNode.name, parent.get(currNode.name - 1).name);
          }
          //For each edge (u, v) incident to u
          for (int i = 0; i < currNode.adjlist.size(); ++i) {
              //Add v to the stack S
              stack.add(currNode.adjlist.get(i));
              parent.set(currNode.adjlist.get(i).name-1, currNode);
              System.out.printf("discover: %d\n", currNode.adjlist.get(i).name);
          }
      }
  }
}

