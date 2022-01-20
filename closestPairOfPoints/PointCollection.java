PointPair closestPairRec(ArrayList<Point> Px, ArrayList<Point> Py) {
// base case start (same as brute force on a 3 point set)
if (Px.size() <= 3) {
  double minDist = Double.MAX_VALUE;  // initialize to some arbitrarily large value to use in initial comparison
  Point temp1 = new Point(0,0);
  Point temp2 = new Point(0,0);
  PointPair minPair = new PointPair(temp1, temp2);

  for (int i = 0; i < Px.size()-1; ++i) {
    for (int j = i+1; j < Px.size(); ++j) {
      if (Px.get(i).distanceTo(Px.get(j)) < minDist) {
        minDist = Px.get(i).distanceTo(Px.get(j));
        minPair.p1 = Px.get(i);
        minPair.p2 = Px.get(j);
      }
    }
  }
  return minPair;
} // end of base case

// Construct Qx, Qy, Rx, Ry
ArrayList<Point> Qx = new ArrayList<Point>();
ArrayList<Point> Qy = new ArrayList<Point>();

for (int i = 0; i < Px.size()/2; ++i) {
  Point p_x = Px.get(i);
  Point p_y = Py.get(i);
  Qx.add(p_x);
  Qy.add(p_y);
}

ArrayList<Point> Rx = new ArrayList<Point>();
ArrayList<Point> Ry = new ArrayList<Point>();
for (int i = Px.size()/2; i < Px.size(); ++i) {
  Point p_x = Px.get(i);
  Point p_y = Py.get(i);
  Rx.add(p_x);
  Ry.add(p_y);
}

PointPair q_star = closestPairRec(Qx, Qy);
PointPair r_star = closestPairRec(Rx, Ry);

double delta = Math.abs(Math.min(q_star.p1.distanceTo(q_star.p2), r_star.p1.distanceTo(r_star.p2)));
double x_star = Qx.get(Qx.size() - 1).x;
double lowerBoundX = x_star - delta;
double upperBoundX = x_star + delta;

// Construct set of points in delta range of midline - in sorted Y order by default based on how theyre added
ArrayList<Point> midLineSet = new ArrayList<Point>();
for (int i = 0; i < Py.size(); ++i) {
  if (Py.get(i).x >= lowerBoundX && Py.get(i).x <= upperBoundX) {
    midLineSet.add(Py.get(i));
  }
}

Point temp1 = new Point(0,0);
Point temp2 = new Point(0,0);
PointPair minPair = new PointPair(temp1, temp2);
double minDist = Double.MAX_VALUE;  // initialize to some arbitrarily large value to use in initial comparison

for (Point p : midLineSet) {
  int index = midLineSet.indexOf(p);
  for (int i = index+1; i < Math.min(index + 16, midLineSet.size()); ++i) {
    if (p.distanceTo(midLineSet.get(i)) <= minDist) {
      minDist = p.distanceTo(midLineSet.get(i));
      // Added this just so it will return the pair in the same order everytime (easier to compare output to brute force)
      if (p.x < midLineSet.get(i).x) {
        minPair.p1 = p;
        minPair.p2 = midLineSet.get(i);
      } else {
        minPair.p2 = p;
        minPair.p1 = midLineSet.get(i);
      }
    }
  }
}

if (minDist < delta) {
  return minPair;
} else if (q_star.p1.distanceTo(q_star.p2) < r_star.p1.distanceTo(r_star.p2)) {
  return q_star;
} else {
  return r_star;
}
}

PointPair closestPair() {
// Create two arrays of the points to be sorted by x and y respectively
ArrayList<Point> Px = new ArrayList<Point>();
ArrayList<Point> Py = new ArrayList<Point>();
for (Point p : points) {
  Px.add(p);
  Py.add(p);
}

// sort them
Collections.sort(Py, Point::compareY);
Collections.sort(Px, Point::compareX);

return closestPairRec(Px, Py);
}

PointPair bruteForce() {
// implement this
double minDist = Double.MAX_VALUE;  // initialize to some arbitrarily large value to use in initial comparison
Point temp1 = new Point(0,0);
Point temp2 = new Point(0,0);

PointPair minPair = new PointPair(temp1, temp2);

for (int i = 0; i < points.size()-1; ++i) {
  temp1 = points.get(i);
  for (int j = i+1; j < points.size(); ++j) {
    temp2 = points.get(j);
    if (temp1.distanceTo(temp2) <= minDist) {
      minDist = temp1.distanceTo(temp2);
      // This conditional is just to standardize the pointPair output for comparison when printing them out.
      if (temp1.x < temp2.x) {
        minPair.p1 = temp1;
        minPair.p2 = temp2;
      } else {
        minPair.p2 = temp1;
        minPair.p1 = temp2;
      }
    }
  }
}
return minPair;
}
}