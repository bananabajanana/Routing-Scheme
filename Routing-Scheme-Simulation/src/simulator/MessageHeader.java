package simulator;

/**
 * The header of every message.
 * Notice that depending on routeType, only some fields should be visible, in the following way:
 * - type=local: serializedS, serializedT
 * - type=toLandmark: serializedS, address
 * - type=fromLandmark/direct: serializedS, serializedT, position, path
 * - type=handshake: serializedS, serializedT, path
 */
public class MessageHeader {
  private RouteType routeType;
  private int serializedS;
  private int serializedT;
  private Address address;
  private int position;
  private int[] path;

  //region <Constructors>

  private MessageHeader() {

  }

  /**
   * Create a new message header of type=local.

   * @param serializedS The serialization of the source node
   * @param serializedT The serialization of the target node
   * @return a corresponding MessageHeader object
   */
  public static MessageHeader newLocalHeader(int serializedS, int serializedT) {
    MessageHeader output = new MessageHeader();
    output.routeType = RouteType.local;
    output.serializedS = serializedS;
    output.serializedT = serializedT;
    return output;
  }

  /**
   * Create a new message header of type=toLandmark.

   * @param serializedS The serialization of the source node
   * @param address The full address of the target node
   * @return a corresponding MessageHeader object
   */
  public static MessageHeader newToLandmarkHeader(int serializedS, Address address) {
    MessageHeader output = new MessageHeader();
    output.routeType = RouteType.toLandmark;
    output.serializedS = serializedS;
    output.address = address;
    return output;
  }

  /**
   * Create a new message header of type=fromLandmark.

   * @param serializedS The serialization of the source node
   * @param serializedT The serialization of the target node
   * @param path The path from source to target
   * @return a corresponding MessageHeader object
   */
  public static MessageHeader newFromLandmarkHeader(int serializedS, int serializedT,
      int[] path) {
    MessageHeader output = new MessageHeader();
    output.routeType = RouteType.fromLandmark;
    output.serializedS = serializedS;
    output.serializedT = serializedT;
    output.path = path;
    output.position = 0;
    return output;
  }

  /**
   * Create a new message header of type=direct.

   * @param serializedS The serialization of the source node
   * @param serializedT The serialization of the target node
   * @param path The path from source to target
   * @return a corresponding MessageHeader object
   */
  public static MessageHeader newDirectHeader(int serializedS, int serializedT, int[] path) {
    MessageHeader output = new MessageHeader();
    output.routeType = RouteType.direct;
    output.serializedS = serializedS;
    output.serializedT = serializedT;
    output.path = path;
    output.position = 0;
    return output;
  }

  /**
   * Create a new message header of type=handshake.

   * @param serializedS The serialization of the source node
   * @param serializedT The serialization of the target node
   * @param path The path from source to target
   * @return a corresponding MessageHeader object
   */
  public static MessageHeader newHandshakeHeader(int serializedS, int serializedT,
      int[] path) {
    MessageHeader output = new MessageHeader();
    output.routeType = RouteType.handshake;
    output.serializedS = serializedS;
    output.serializedT = serializedT;
    output.path = path;
    return output;
  }
  //endregion

  //region <Getters and Setters>

  public RouteType getRouteType() {
    return routeType;
  }

  public int getSerializedS() {
    return serializedS;
  }

  /**
   * MUST NOT BE type='toLandmark'.

   * @return serializedT value
   */
  public int getSerializedT() {
    if (routeType == RouteType.toLandmark) {
      throw new RuntimeException("HEADER: toLandmark header tried to access T serialization");
    }

    return serializedT;
  }

  /**
   * MUST BE type='toLandmark'.

   * @return address
   */
  public Address getAddress() {
    if (routeType != RouteType.toLandmark) {
      throw new RuntimeException("HEADER: header which is not toLandmark tried to access address");
    }

    return address;
  }

  /**
   * MUST BE type='fromLandmark' or 'direct'.

   * @return path position
   */
  public int getPosition() {
    if (routeType != RouteType.fromLandmark && routeType != RouteType.direct) {
      throw new RuntimeException("HEADER: header which is not 'fromLandmark' or 'direct' tried to "
          + "access position");
    }

    return position;
  }

  /**
   * Increments the position value bt one.
   */
  public void incrementPosition() {
    if (routeType != RouteType.fromLandmark && routeType != RouteType.direct) {
      throw new RuntimeException("HEADER: header which is not 'fromLandmark' or 'direct' tried to "
          + "access position");
    }

    position++;
  }

  /**
   * MUST NOT BE type='local' or 'toLandmark'.

   * @return path from source to target
   */
  public int[] getPath() {
    if (routeType == RouteType.local || routeType == RouteType.toLandmark) {
      throw new RuntimeException("HEADER: header which is 'loca' or 'toLandmark' tried to access "
          + "path");
    }

    return path;
  }

  //endregion
}
