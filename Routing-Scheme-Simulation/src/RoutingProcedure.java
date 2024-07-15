import org.jgrapht.Graph;
import org.jgrapht.graph.DefaultEdge;
import simulator.ComputerNode;
import simulator.Message;
import simulator.MessageHeader;
import simulator.RouteType;
import simulator.RoutingGraphBuilder.Print;

/**
 * A simulation of the message forwarding procedure on a pre-processed graph.
 */
public class RoutingProcedure {
  private Graph<ComputerNode, DefaultEdge> network;
  private static RoutingProcedure instance;

  private RoutingProcedure() {}

  /**
   * Singleton instance creation and getter.

   * @return Instance of the class
   */
  public static RoutingProcedure getInstance() {
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
    boolean local = s.getBall().contains(t);
    ComputerNode msgPosition;
    int hops = 0;
    Message message;

    if (local) {
      message = new Message(MessageHeader.newLocalHeader(s.getNodeIndex(), t.getNodeIndex()));
      msgPosition = s.getPorts().get(s.getTbl().get(t.getNodeIndex()));
      hops++;
    } else {
      if (t.getAddress().getClosestLandmarkId() != s.getNodeIndex()) {
        message = new Message(MessageHeader.newToLandmarkHeader(s.getNodeIndex(), t.getAddress()));
        msgPosition = s.getPorts().get(s.getTbl().get(t.getAddress().getClosestLandmarkId()));
        hops++;
      } else {
        message = new Message(MessageHeader.newFromLandmarkHeader(s.getNodeIndex(),
            t.getNodeIndex(), t.getAddress().getFromLandmarkPath()));

        msgPosition = s.getPorts().get(
            message.getHeader().getPath()[message.getHeader().getPosition()]);
      }
    }

    while (msgPosition != t) {
      if (message.getHeader().getRouteType() == RouteType.local) {
        msgPosition = msgPosition.getPorts().get(msgPosition.getTbl().get(t.getNodeIndex()));
      } else if (message.getHeader().getRouteType() == RouteType.toLandmark) {
        if (msgPosition.getNodeIndex() == message.getHeader().getAddress().getClosestLandmarkId()) {
          message.setHeader(MessageHeader.newFromLandmarkHeader(
              message.getHeader().getSerializedS(),
              message.getHeader().getAddress().getNodeId(),
              message.getHeader().getAddress().getFromLandmarkPath()));

          msgPosition = msgPosition.getPorts().get(
              message.getHeader().getPath()[message.getHeader().getPosition()]);
        } else {
          msgPosition = msgPosition.getPorts().get(msgPosition.getTbl().get(
              t.getAddress().getClosestLandmarkId()));
        }
      } else if (message.getHeader().getRouteType() == RouteType.fromLandmark) {
        message.getHeader().incrementPosition();
        msgPosition = msgPosition.getPorts().get(
            message.getHeader().getPath()[message.getHeader().getPosition()]);
      }
      hops++;
    }

    return hops;
  }

  private double expectedVal(float[] values) {
    double sum = 0;
    for (int i = 0; i < values.length; i++) {
      sum += values[i];
    }
    sum /= values.length;
    return sum;
  }

  private double varianceVal(float[] values) {
    double sum = 0;
    for (int i = 0; i < values.length; i++) {
      sum += values[i];
    }
    sum /= values.length;
    double output = 0;
    for (int i = 0; i < values.length; i++) {
      output += ((values[i] - sum) * (values[i] - sum)) / (values.length - 1);
    }
    return output;
  }

  /**
   * Calculates the expected value and variance of the stretch value for a series of nodes.
   * Does this while ignoring the handshake procedure.
   * Also prints all info on pairs in CSV format.

   * @param s An array of all the source nodes
   * @param t An array of all the target nodes
   * @param spLengths An array of the precalculated shortest paths between them
   * @return An array of the format { Exp, Var }
   */
  public double[] expVarNoHandshakes(ComputerNode[] s, ComputerNode[] t, int[] spLengths,
      Print csvPrint) {
    if (csvPrint == Print.Detailed) {
      System.out.println("s,t,d,msg,stretch");
    }
    float[] msgDistance = new float[s.length];
    float[] stretches = new float[s.length];
    for (int i = 0; i < s.length; i++) {
      msgDistance[i] = sendMessageNoHandshakes(s[i], t[i]);
      stretches[i] = ((float) msgDistance[i]) / ((float) spLengths[i]);

      if (csvPrint == Print.Detailed) {
        System.out.println(
            s[i].getNodeIndex() + "," + t[i].getNodeIndex() + "," + spLengths[i] + ","
                + msgDistance[i] + "," + stretches[i]);
      }
    }

    double[] output = { expectedVal(stretches), varianceVal(stretches) };
    if (csvPrint != Print.NONE) {
      System.out.println(output[0] + "," + output[1]);
    }

    return output;
  }

  /**
   * Calculates the expected value and variance of the stretch value for a series of nodes.
   * Does this while 'flattening' the handshake procedure (every two nodes that would've gone
   * through handshake, are calculated after the process)
   * Also prints all info on pairs in CSV format.

   * @param s An array of all the source nodes
   * @param t An array of all the target nodes
   * @param spLengths An array of the precalculated shortest paths between them
   * @return An array of the format { Exp, Var }
   */
  public double[] expVarWithHandshakes(ComputerNode[] s, ComputerNode[] t, int[] spLengths,
      boolean csvPrint) {
    if (csvPrint) {
      System.out.println("s,t,d,msg,stretch");
    }
    float[] msgDistance = new float[s.length];
    float[] stretches = new float[s.length];
    for (int i = 0; i < s.length; i++) {
      boolean thirdCaseShouldSkip = (!s[i].getBall().contains(t[i]))
          && (t[i].getBall().contains(s[i]));
      if (thirdCaseShouldSkip) {
        stretches[i] = 1;
        msgDistance[i] = spLengths[i];
      } else {
        msgDistance[i] = sendMessageNoHandshakes(s[i], t[i]);
        stretches[i] = ((float) msgDistance[i]) / ((float) spLengths[i]);
      }
      if (csvPrint) {
        System.out.println(s[i].getNodeIndex() + "," + t[i].getNodeIndex() + "," + spLengths[i]
            + "," + msgDistance[i] + "," + stretches[i]);
      }
    }

    double[] output = { expectedVal(stretches), varianceVal(stretches) };
    if (csvPrint) {
      System.out.println("");
      System.out.println("Stretch: " + output[0] + " ± " + output[1]);
      System.out.println("");
      System.out.println("");
    }

    return output;
  }
}
