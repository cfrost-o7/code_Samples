public int kruskal() {
    System.out.println("\nMinimum Span Edges: ");
    Node curr = this.nodes.get(0);
    Node next = null;
    ArrayList<Edge> edges = new ArrayList<Edge>();

    for (int i = 0; i < this.nodes.size(); ++i) {
      for (int j = 0; j < curr.adjlist.size(); ++j) {
        next = curr.adjlist.get(j).n2;
        int weight = curr.adjlist.get(j).weight;

        // This block just keeps the edge construction uniform, smaller node first
        Edge currEdge;
        if (curr.name < next.name) {
          currEdge = new Edge(curr, next, weight);
        } else {
          currEdge = new Edge(next, curr, weight);
        }

        // Only adds new edges (avoids adding the edge back to a node we've already explored so no duplicates)
        boolean exists = false;
        for (Edge edge : edges) {
          if (edge.compareTo(currEdge) == 0) {
            exists = true;
            break;
          }
        }
        if (!exists) {
          edges.add(currEdge);
        }
      }
      curr = nodes.get(i);
    }

    // Sort edges by weight
    Collections.sort(edges);

    // Initialize empty list of minimum edges - can't be more than numNodes - 1
    ArrayList<Edge> minEdges = new ArrayList<Edge>(nodes.size() - 1);

    UnionFind components = new UnionFind(this);
    int index = 0;
    int totalWeight = 0;

    // minimum spanning tree cannot have more edges than numNodes-1 so use that to stop loop
    while (minEdges.size() < nodes.size() - 1) {
      // If they aren't equal then they're not in the same component and can add
      if (components.Find(edges.get(index).n1) != components.Find(edges.get(index).n2)) {
        minEdges.add(edges.get(index));
        components.Union(edges.get(index).n1, edges.get(index).n2);

        totalWeight += edges.get(index).weight;
        System.out.println("add edge " + edges.get(index).toString());
      }
      ++index;
    }

    return totalWeight;
}

