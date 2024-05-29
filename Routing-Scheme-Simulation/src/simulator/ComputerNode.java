package simulator;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

/**
 * Additional data each node saves in the graph.
 */
public class ComputerNode {
  private final int nodeIndex;
  private Address address;
  private final ArrayList<ComputerNode> ports;
  private final HashMap<Integer, Integer> tbl;
  private int neighborCounter;
  private final HashMap<ComputerNode, List<ComputerNode>> shortestPathsToLandmarks;
  private ComputerNode closestLandmark; // l(v)
  private double distanceToClosestLandmark;  // for ball calculations
  private final ArrayList<ComputerNode> ball;

  /**
   * Empty Computer Node constructor.
   * Initiating a node with only its index number in the graph,
   * all other parameters are empty.
   *
   * @param index a number representing the order degree in the nodes creation process of a graph
   */
  public ComputerNode(int index) {
    nodeIndex = index;
    ports = new ArrayList<>();
    tbl = new HashMap<>();
    shortestPathsToLandmarks = new HashMap<>();
    neighborCounter = 0;
    closestLandmark = null;
    distanceToClosestLandmark = Double.MAX_VALUE;
    ball = new ArrayList<>();
  }

  public void connectPort(ComputerNode neighbor) {
    ports.add(neighborCounter, neighbor);
    ++neighborCounter;
  }

  public void insertNodeToBall(ComputerNode v) {
    ball.add(v);
  }

  public void insertShortestPathToLandmark(ComputerNode landmark, List<ComputerNode> shortestPath) {
    shortestPathsToLandmarks.put(landmark, shortestPath);
  }

  //region <Getters>

  /**
   * Gets the port index that is assinged to the given neighbor of this node.

   * @param neighbor a node connected with an edge to this node
   * @return port index
   * @throws RuntimeException if a node that isn't a neighbor of this node is given.
   */
  public int getPortIndexOfNeighbor(ComputerNode neighbor) {
    int index = ports.indexOf(neighbor);
    if (index == -1) {
      throw new RuntimeException("The given ComputerNode is not a neighbor");
    }
    return index;
  }

  public int getPortForGettingToTargetNode(ComputerNode target) {
    return tbl.get(target.getNodeIndex());
  }

  public List<ComputerNode> getShortestPathToLandmark(ComputerNode landmark) {
    return shortestPathsToLandmarks.get(landmark);
  }

  public ArrayList<ComputerNode> getBall() {
    return ball;
  }

  public int getDegree() {
    return neighborCounter;
  }

  public HashMap<Integer, Integer> getTbl() {
    return tbl;
  }

  public HashMap<ComputerNode, List<ComputerNode>> getShortestPathsToLandmarks() {
    return shortestPathsToLandmarks;
  }

  public ComputerNode getClosestLandmark() {
    return closestLandmark;
  }

  public double getDistanceToClosestLandmark() {
    return distanceToClosestLandmark;
  }

  public int getNodeIndex() {
    return nodeIndex;
  }

  public Address getAddress() {
    return address;
  }

  public ArrayList<ComputerNode> getPorts() {
    return ports;
  }
  //endregion

  //region <Getters>

  /**
   * Inserts a key-value pair to this node table.
   * Sets the target node as the key.
   * Sets the port number connected to the neighbor of this node that is
   * the next step on shortest path to the target node as the value.
   *
   * @param target         node that there is a path leads to from this node.
   * @param neighborInPath next node on shortest path from this node to target
   */
  public void setNeighborPortInPathToNodeInTbl(ComputerNode target, ComputerNode neighborInPath) {
    tbl.put(target.getNodeIndex(), getPortIndexOfNeighbor(neighborInPath));
  }

  /**
   * Sets the distance to the closest landmark (maxing underflows and errors).

   * @param distance calculated distance
   */
  public void setDistanceToClosestLandmark(double distance) {
    if (distance < 0) {
      distanceToClosestLandmark = Double.MAX_VALUE;
    } else {
      distanceToClosestLandmark = distance;
    }
  }

  public void setClosestLandmark(ComputerNode landmark) {
    closestLandmark = landmark;
  }

  public void setAddress(Address address) {
    this.address = address;
  }

  //endregion
}
