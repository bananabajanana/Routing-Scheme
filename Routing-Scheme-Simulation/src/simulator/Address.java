package simulator;

/**
 * The full address of a node, which contains meta-data about the graph post instantiation phase.
 * The address contains:
 * - nodeID: The original node identifier.
 * - closestLandmarkID: The identifier of the closest landmark to the node
 * - fromLandmarkPath: the routes taken along the path from said landmark to the node
 * - a bit representation of the above (might be replaced eventually with serialization function)
 * - a mask for the bit representation
 */
public class Address {
  private int nodeID;
  private int closestLandmarkID;
  private int[] fromLandmarkPath;
  private double fullAddressRepresentation;
  private double addressMask;

  /**
   * Constructor for Address class.

   * @param nodeID The serialized node this is the address of
   * @param closestLandmarkID closest landmark to node
   * @param fromLandmarkPath the path from said landmark to this node
   */
  public Address(int nodeID, int closestLandmarkID,
      int[] fromLandmarkPath) {
    this.nodeID = nodeID;
    this.closestLandmarkID = closestLandmarkID;
    this.fromLandmarkPath = fromLandmarkPath;
  }

  //region <Getters and Setters>

  public void setNodeID(int nodeID) {
    this.nodeID = nodeID;
  }

  public void setClosestLandmarkID(int closestLandmarkID) {
    this.closestLandmarkID = closestLandmarkID;
  }

  public void setFromLandmarkPath(int[] fromLandmarkPath) {
    this.fromLandmarkPath = fromLandmarkPath;
  }

  public int getNodeID() {
    return nodeID;
  }

  public int getClosestLandmarkID() {
    return closestLandmarkID;
  }

  public int[] getFromLandmarkPath() {
    return fromLandmarkPath;
  }

  public double getFullAddressRepresentation() {
    return fullAddressRepresentation;
  }

  public double getAddressMask() {
    return addressMask;
  }
  //endregion


}
