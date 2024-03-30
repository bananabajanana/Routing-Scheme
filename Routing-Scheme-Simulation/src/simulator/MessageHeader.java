package simulator;

/**
 * The header of every message.
 * Notice that depending on routeType, only some fields should be visible, in the following way:
 * - type=local: serializedS, serializedT
 * - type=toLandmark: serializedS, address
 * - type=fromLandmark/direct: serializedS, serializedT, position, pathFromLandmark
 * - type=handshake: serializedS, serializedT, pathFromLandmark
 */
public class MessageHeader {
  private RouteType routeType;
  private String serializedS;
  private String serializedT;
  private Address address;
  private int position;
  private int[] pathFromLandmark;
}
