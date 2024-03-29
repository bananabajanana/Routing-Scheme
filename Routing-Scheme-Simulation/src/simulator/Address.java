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
  int[] fromLandmarkPath;
  double fullAddressRepresentation;
  double addressMask;
}
