public void BellmanFord(Node t) {
    //n = number of nodes in G
    int n = nodes.size();

    //Array M[0 . . . n − 1, V]
    // made n+1 rows so it can use node name as the row num - row 0 is also used by convention based on class for t node distance
    int[][] M = new int[n+1][n];

    //Define M[0, t]= 0 and M[0, v]= ∞ for all other v ∈ V
    for (int c = 1; c < n; ++c) {
        M[c][0] = 1000000;
    }

    //For i = 1, . . . , n − 1   - this is the iterations
    for (int j = 1; j < n; ++j) {
        //For v ∈ V in any order  - this is the nodes
        for (Node v : nodes) {
            // loop over adjlist and update min to lowest of all connected nodes
            int min = 1000000;
            for (Link link : v.adjlist) {
                int currVal = M[link.n2.name][j-1] + link.weight;
                if (currVal < min) {
                    min = currVal;
                }
            }
            // now update currNode iteration val with min
            M[v.name][j] = Math.min(M[v.name][j-1], min);
            //Endfor
        }
    //Endfor
    }

    // This is just to print out the iterations table for confirming correctness
    for(int i = 0; i < nodes.size(); ++i) {
        for (int j = 0; j < nodes.size(); ++j) {
            System.out.format("%d,", M[i][j]);
        }
        System.out.print("\n");
    }
}
