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
  private int nodeId;
  private int closestLandmarkId;

  // the port path from the closest landmark to this node to the node itself
  private int[] fromLandmarkPath;
  private double fullAddressRepresentation;
  private double addressMask;

  /**
   * Constructor for Address class.

   * @param nodeId The serialized node this is the address of
   * @param closestLandmarkId closest landmark to node
   * @param fromLandmarkPath the path from said landmark to this node
   */
  public Address(int nodeId, int closestLandmarkId,
      int[] fromLandmarkPath) {
    this.nodeId = nodeId;
    this.closestLandmarkId = closestLandmarkId;
    this.fromLandmarkPath = fromLandmarkPath;
  }

  //region <Getters and Setters>

  public void setNodeId(int nodeId) {
    this.nodeId = nodeId;
  }

  public void setClosestLandmarkId(int closestLandmarkId) {
    this.closestLandmarkId = closestLandmarkId;
  }

  public void setFromLandmarkPath(int[] fromLandmarkPath) {
    this.fromLandmarkPath = fromLandmarkPath;
  }

  public int getNodeId() {
    return nodeId;
  }

  public int getClosestLandmarkId() {
    return closestLandmarkId;
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
