package simulator;

/**
 * The full address of a node, which contains meta-data about the graph post instantiation phase.
 * The address contains:
 * - The original node identifier.
 * - The identifier of the closest landmark to the node
 * - the routes taken along the path from said landmark to the node
 * - a bit representation of the above (might be replaced eventually with serialization function)
 * - a mask for the bit representation
 */
public class Address {
  private String nodeSerialization;
  private String closestLandmarkSerialization;
  private int[] fromLandmarkPath;
  private double fullAddressRepresentation;
  private double addressMask;

  /**
   * Constructor for Address class.

   * @param nodeSerialization The serialized node this is the address of
   * @param closestLandmarkSerialization closest landmark to node
   * @param fromLandmarkPath the path from said landmark to this node
   */
  public Address(String nodeSerialization, String closestLandmarkSerialization,
      int[] fromLandmarkPath) {
    this.nodeSerialization = nodeSerialization;
    this.closestLandmarkSerialization = closestLandmarkSerialization;
    this.fromLandmarkPath = fromLandmarkPath;
  }

  //region <Getters and Setters>

  public void setNodeSerialization(String nodeSerialization) {
    this.nodeSerialization = nodeSerialization;
  }

  public void setClosestLandmarkSerialization(String closestLandmarkSerialization) {
    this.closestLandmarkSerialization = closestLandmarkSerialization;
  }

  public void setFromLandmarkPath(int[] fromLandmarkPath) {
    this.fromLandmarkPath = fromLandmarkPath;
  }

  public String getNodeSerialization() {
    return nodeSerialization;
  }

  public String getClosestLandmarkSerialization() {
    return closestLandmarkSerialization;
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
