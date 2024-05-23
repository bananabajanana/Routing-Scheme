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
    float output = 0;
    for (int i = 0; i < values.length; i++) {
      output += ((values[i] - sum) * (values[i] - sum)) / (values.length - 1);
    }
    return output;
  }

  /**
   * Calculates the expected value and variance of the stretch value for a series of nodes.
   * Does this while ignoring the handshake procedure.

   * @param s An array of all the source nodes
   * @param t An array of all the target nodes
   * @param spLengths An array of the precalculated shortest paths between them
   * @return An array of the format { Exp, Var }
   */
  public float[] expVarNoHandshakes(ComputerNode[] s, ComputerNode[] t, int[] spLengths) {
    float[] stretches = new float[s.length];
    for (int i = 0; i < s.length; i++) {
      stretches[i] = ((float) sendMessageNoHandshakes(s[i], t[i])) / ((float) spLengths[i]);
    }

    float[] output = { expectedVal(stretches), varianceVal(stretches) };
    return output;
  }

  /**
   * Calculates the expected value and variance of the stretch value for a series of nodes.
   * Does this while 'flattening' the handshake procedure (every two nodes that would've gone
   * through handshake, are calculated after the process)

   * @param s An array of all the source nodes
   * @param t An array of all the target nodes
   * @param spLengths An array of the precalculated shortest paths between them
   * @return An array of the format { Exp, Var }
   */
  public float[] expVarWithHandshakes(ComputerNode[] s, ComputerNode[] t, int[] spLengths) {
    float[] stretches = new float[s.length];
    for (int i = 0; i < s.length; i++) {
      boolean thirdCaseShouldSkip = false; //TODO: implement check
      if (thirdCaseShouldSkip) {
        stretches[i] = 1;
      } else {
        stretches[i] = ((float) sendMessageNoHandshakes(s[i], t[i])) / ((float) spLengths[i]);
      }
    }

    float[] output = { expectedVal(stretches), varianceVal(stretches) };
    return output;
  }
}
