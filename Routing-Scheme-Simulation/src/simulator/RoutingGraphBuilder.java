package simulator;

import java.util.ArrayList;
import java.util.List;
import org.jgrapht.Graph;
import org.jgrapht.GraphPath;
import org.jgrapht.alg.interfaces.ShortestPathAlgorithm;
import org.jgrapht.alg.shortestpath.BFSShortestPath;
import org.jgrapht.graph.DefaultEdge;
import simulator.graphs.GraphWithCore;


/**
 * Builder class for setting up the simulator graphs.
 */
public class RoutingGraphBuilder {
  private final GraphWithCore graphWithCore;
  private ArrayList<ComputerNode> core;
  private static final int SINGLE_VERTEX_PATH_LEN = 0;


  //region <Constructors>

  public RoutingGraphBuilder(GraphWithCore graph) {
    this.graphWithCore = graph;
    this.core = new ArrayList<>();
  }

  //endregion

  // region <Preprocessing>

  /**
   * For a given node v this method iterate on every vertex in the graphs core,
   * (AKA landmarks), gets a shortest path to each landmark and save in the table of v
   * the port index of the neighbor in a shortest path to each landmark from v is saved in
   * the table member of v.

   * All the shortest paths from v to each landmark are saved in v.

   * The closest landmark and its distance from v are saved in v.

   * When v is a landmark:
   *  -   the closest landmark of a landmark is itself
   *  -   the shortest path from a landmark to itself is just itself
   *  -   the distance from a landmark to its closest landmark (i.e. itself) is zero
   *  -   in the table of a landmark there is no entry for a port that's connected to itself
   *      (that means that if v is a landmark then there is no entry in its tbl for itself)
   *
   * If there is no path from v to any landmark then the "closestLandmark" member
   * field of v will remain null and the "distanceToClosestLandmark" member field
   * will have the value of Double.POSITIVE_INFINITY.
   * (this case happens when v is a vertex in a connectivity component that is
   * not connected to any connectivity component that have a landmark in it)
   *
   *
   * @param v a vertex in the graph
   * @param shortestPathsFromV all the shortest paths from v to every other node in the graph
   */
  private void processCore(ComputerNode v, ShortestPathAlgorithm.SingleSourcePaths<ComputerNode,
      DefaultEdge> shortestPathsFromV) {
    double minDistFromNodeToCore = Double.POSITIVE_INFINITY;
    ComputerNode closestLandmarkToNode = null;
    for (ComputerNode landmark : core){
      GraphPath<ComputerNode, DefaultEdge> path = shortestPathsFromV.getPath(landmark);
      if (path != null) { // check if there is a path from v to the current landmark
        List<ComputerNode> shortestPathToLandmark = shortestPathsFromV.getPath(landmark).getVertexList();
        int distFromCurrLandmark = shortestPathToLandmark.size() - 1;   // minus 1 for the num of edges

        if (distFromCurrLandmark > SINGLE_VERTEX_PATH_LEN) {    // check if v is the current landmark
          shortestPathToLandmark.get(0).setNeighborPortInPathToNodeInTbl(landmark, shortestPathToLandmark.get(1));
        }

        v.insertShortestPathToLandmark(landmark, shortestPathToLandmark);

        if (minDistFromNodeToCore > distFromCurrLandmark) {
          minDistFromNodeToCore = distFromCurrLandmark;
          closestLandmarkToNode = landmark;
        }
      }
    }
    v.setClosestLandmark(closestLandmarkToNode);
    v.setDistanceToClosestLandmark(minDistFromNodeToCore);
  }

  /**
   * Iterating on all the graphs vertexes while looking for vertexes that should
   * be included in the Ball of the given vertex v.
   *
   * A vertex is in the ball of v if and only if the distance of it from v is smaller
   * than the distance of v from the closest landmark to v.
   * (i.e. vertex u is in ball(v) if and only if d(v,u) < d(v,l(u))
   * the vertex v itself is not included in its Ball (even tho by the definition it should be I guess)
   *
   * If a there is no paths from any Landmark in the graphs core to vertex v then the ball of v will be empty.
   * (the distance from v to the core wil be Double.POSITIVE_INFINITY)
   *
   * @param v the vertex in the middle of the ball
   * @param pathsFromV all the shortest paths from v to every other vertex in the graph
   */
  private void findAndProcessBallOfNode(ComputerNode v, ShortestPathAlgorithm.SingleSourcePaths<ComputerNode, DefaultEdge> pathsFromV){
    for (ComputerNode u : graphWithCore.getGraph().vertexSet()){
      if (u != v ){
        GraphPath<ComputerNode, DefaultEdge> path = pathsFromV.getPath(u);
        if (path != null) { // check if there is a path from v to u
          List<ComputerNode> shortestPathToU = path.getVertexList();
          int lenOfShortestPathToU = shortestPathToU.size() - 1;
          if (lenOfShortestPathToU < v.getDistanceToClosestLandmark()) {
            if (shortestPathToU.size() > SINGLE_VERTEX_PATH_LEN) {
              shortestPathToU.get(0).setNeighborPortInPathToNodeInTbl(u, shortestPathToU.get(1));
            }
            v.insertNodeToBall(u);
          }
        }
      }
    }
  }

  /**
   * Returns an int array for the ports that are used in a shortest path from
   * the closest landmark to v to the vertex v itself.
   *
   * @param v a vertex in the graph.
   * @return an empty array if there is no path to v from the landmarks.
   */
  private int[] getRevPortPathFromClosestLandmark(ComputerNode v){
    ComputerNode closestLandmarkToV = v.getClosestLandmark();
    List<ComputerNode> path = v.getShortestPathToLandmark(closestLandmarkToV);
    if (path == null){
      return new int[0];
    }
    int len = path.size() - 1;
    int[] revPortPath = new int[len]; // no need for port in the last node (that is the target node)

    for (int i = 0; i < len; i++){
      revPortPath[i] = path.get(len - i).getPortIndexOfNeighbor(path.get(len - i - 1));
    }
    return revPortPath;
  }


  /**
   * Creates a new Address object for v.
   *
   *  -  if there is no path from v to any landmark then the closest landmark index
   *      member of the Address object will be -1 and the port array will be empty!
   * @param v a vertex in the graph.
   */
  private void initAddress(ComputerNode v){
    int[] portPathFromClosestLandmarkToV = getRevPortPathFromClosestLandmark(v);
    int closestLandmarkIndex = v.getClosestLandmark() != null ? v.getClosestLandmark().getNodeIndex() : -1;
    v.setAddress(new Address(v.getNodeIndex(),closestLandmarkIndex, portPathFromClosestLandmarkToV));
  }

  private void initiatePorts() {
    Graph<ComputerNode, DefaultEdge> graph = graphWithCore.getGraph();
    for(DefaultEdge edge : graph.edgeSet()) {
      ComputerNode u = graph.getEdgeSource(edge);
      ComputerNode v = graph.getEdgeTarget(edge);
      u.connectPort(v);
      v.connectPort(u);
    }
  }
  private float expectedVal(float[] values) {
    float sum = 0;
    for (int i = 0; i < values.length; i++) {
      sum += values[i];
    }
    sum /= values.length;
    return sum;
  }

  private float varianceVal(float[] values) {
    float sum = 0;
    for (int i = 0; i < values.length; i++) {
      sum += values[i];
    }
    sum /= values.length;
    double output = 0;
    for (int i = 0; i < values.length; i++) {
      output += ((values[i] - sum) * (values[i] - sum)) / (values.length - 1);
    }

    output = Math.sqrt(output);

    return (float)output;
  }

  public void process(boolean csvPrint){
    initiatePorts();
    this.core = this.graphWithCore.getCore();
    BFSShortestPath<ComputerNode, DefaultEdge> bfs = new BFSShortestPath<>(graphWithCore.getGraph());

    if(csvPrint) {
      System.out.println("u,tblLines");
    }
    float tableSizes[] = new float[graphWithCore.getGraph().vertexSet().size()];
    int i = 0;

    for (ComputerNode v : graphWithCore.getGraph().vertexSet()) {
      ShortestPathAlgorithm.SingleSourcePaths<ComputerNode, DefaultEdge> shortestPathsFromV =bfs.getPaths(v);
      processCore(v, shortestPathsFromV);
      findAndProcessBallOfNode(v,shortestPathsFromV);
      initAddress(v);
      if(csvPrint) {
        System.out.println(v.getNodeIndex() + "," + v.getTbl().size());
        tableSizes[i++] = v.getTbl().size();
      }
    }

    if(csvPrint) {
      System.out.println("");
      System.out.println("Amount of Table Lines per Node: " + expectedVal(tableSizes) + " ± " + varianceVal(tableSizes));
      System.out.println("");
      System.out.println("");
    }
  }
  //endregion

  /**
   * Generates a small positive double value.
   * @param scaleFactor Determines the upper limit of the epsilon value.
   * @return A small positive double near the scaleFactor magnitude.
   */
  private static double generateEpsilon(double scaleFactor) {
    return Math.random() * scaleFactor;
  }
}
