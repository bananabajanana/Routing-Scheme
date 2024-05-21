import org.jgrapht.Graph;
import org.jgrapht.graph.DefaultEdge;
import simulator.ComputerNode;
import simulator.Message;
import simulator.MessageHeader;
import simulator.RouteType;

/**
 * A simulation of the message forwarding procedure on a pre-processed graph.
 */
public class RoutingProcedure {
  private Graph<ComputerNode, DefaultEdge> network;
  private RoutingProcedure instance;

  private RoutingProcedure() {}

  /**
   * Singleton instance creation and getter.

   * @return Instance of the class
   */
  public RoutingProcedure getInstance() {
    if (instance == null) {
      instance = new RoutingProcedure();
    }
    return instance;
  }

  public void setNetwork(Graph<ComputerNode, DefaultEdge> network) {
    this.network = network;
  }

  /**
   * Simulation of sending a single message from s to t (without handshakes).

   * @param s Source node
   * @param t Target node
   * @return The amount of hops on the path from s to t
   */
  public int sendMessageNoHandshakes(ComputerNode s, ComputerNode t) {
    boolean local = true; //TODO: replace with t\in Ball(s)
    ComputerNode msgPosition;
    int hops = 0;
    Message message;

    if (local) {
      message = new Message(MessageHeader.newLocalHeader(s.getNodeIndex(), t.getNodeIndex()));
      msgPosition = s.getPorts().get(s.getTbl().get(t));
      hops++;
    } else {
      message = new Message(MessageHeader.newToLandmarkHeader(s.getNodeIndex(), t.getAddress()));
      msgPosition = s.getPorts().get(s.getTbl().get(t.getAddress().getClosestLandmarkID()));
      hops++;
    }

    while (msgPosition != t) {
      if (message.getHeader().getRouteType() == RouteType.local) {
        msgPosition = msgPosition.getPorts().get(s.getTbl().get(t));
      } else if (message.getHeader().getRouteType() == RouteType.toLandmark) {
        if (msgPosition.getNodeIndex() == message.getHeader().getAddress().getClosestLandmarkID()) {
          message.setHeader(MessageHeader.newFromLandmarkHeader(
              message.getHeader().getSerializedS(),
              message.getHeader().getAddress().getNodeID(),
              message.getHeader().getPath()));

          msgPosition = msgPosition.getPorts().get(
              message.getHeader().getPath()[message.getHeader().getPosition()]);
        } else {
          msgPosition = msgPosition.getPorts().get(s.getTbl().get(
              t.getAddress().getClosestLandmarkID()));
        }
      } else if (message.getHeader().getRouteType() == RouteType.fromLandmark) {
        msgPosition = msgPosition.getPorts().get(
            message.getHeader().getPath()[message.getHeader().getPosition()]);
      }
      hops++;
    }

    return hops;
  }
}
