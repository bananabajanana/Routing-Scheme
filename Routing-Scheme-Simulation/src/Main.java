import java.util.ArrayList;
import java.util.Random;
import org.jgrapht.Graph;
import org.jgrapht.alg.interfaces.ShortestPathAlgorithm;
import org.jgrapht.alg.shortestpath.BFSShortestPath;
import org.jgrapht.graph.DefaultEdge;
import org.jgrapht.graph.DefaultUndirectedGraph;
import simulator.ComputerNode;
import simulator.RoutingGraphBuilder;
import simulator.graphs.AdaptedFdrg;
import simulator.graphs.ManualGraphCore;

/**
 * Temporary Main class.
 * Includes the main function that will run on compilation.
 */
public class Main {

  /**
   * Demo of the tests and analysis.

   * @param args noop
   */
  public static void main(String[] args) {
    rplgTest();
    //manualTest();
  }

  /**
   * Demo of testing on a random graph.
   */
  public static void rplgTest() {
    RoutingProcedure rp = RoutingProcedure.getInstance();

    Random rand = new Random();
    int n = 1000;
    float tau = 2.5f;
    int sampleSize = 250;
    AdaptedFdrg testGraph = new AdaptedFdrg(n, tau);

    RoutingGraphBuilder testBuilder = new RoutingGraphBuilder(testGraph);
    testBuilder.process(true);

    ComputerNode[] s = new ComputerNode[sampleSize];
    ComputerNode[] t = new ComputerNode[sampleSize];
    int[] spLength = new int [sampleSize];

    BFSShortestPath<ComputerNode, DefaultEdge> bfs = new BFSShortestPath<>(testGraph.getGraph());
    ComputerNode[] nodes = testGraph.getGraph().vertexSet().toArray(new ComputerNode[0]);
    for (int i = 0; i < sampleSize; i++) {
      int tempS = rand.nextInt(n);
      int tempT = rand.nextInt(n);
      for (int ii = 0; ii < n; ii++) {
        if (nodes[ii].getNodeIndex() == tempS) {
          s[i] = nodes[ii];
        }
        if (nodes[ii].getNodeIndex() == tempT) {
          t[i] = nodes[ii];
        }
      }
      ShortestPathAlgorithm.SingleSourcePaths<ComputerNode, DefaultEdge> shortestPathsFromV =
          bfs.getPaths(s[i]);
      if (shortestPathsFromV.getPath(t[i]) == null) {
        i--;
      } else {
        spLength[i] = shortestPathsFromV.getPath(t[i]).getLength();
      }
    }

    rp.expVarWithHandshakes(s, t, spLength, true);
  }

  /**
   * Demo of testing on a specific given graph.
   */
  public static void manualTest() {


    Graph<ComputerNode, DefaultEdge> test = new DefaultUndirectedGraph<>(DefaultEdge.class);

    //region <Vertices>
    ComputerNode a = new ComputerNode(1);
    test.addVertex(a);
    ComputerNode c = new ComputerNode(3);
    test.addVertex(c);
    ComputerNode b = new ComputerNode(2);
    test.addVertex(b);
    ComputerNode d = new ComputerNode(4);
    test.addVertex(d);
    ComputerNode e = new ComputerNode(5);
    test.addVertex(e);
    ComputerNode f = new ComputerNode(6);
    test.addVertex(f);
    ComputerNode g = new ComputerNode(7);
    test.addVertex(g);
    ComputerNode h = new ComputerNode(8);
    test.addVertex(h);
    ComputerNode i = new ComputerNode(9);
    test.addVertex(i);
    ComputerNode j = new ComputerNode(10);
    test.addVertex(j);
    ComputerNode k = new ComputerNode(11);
    test.addVertex(k);
    ComputerNode l = new ComputerNode(12);
    test.addVertex(l);
    ComputerNode m = new ComputerNode(13);
    test.addVertex(m);
    ComputerNode n = new ComputerNode(14);
    test.addVertex(n);
    ComputerNode o = new ComputerNode(15);
    test.addVertex(o);
    ComputerNode p = new ComputerNode(16);
    test.addVertex(p);
    //endregion

    //region <Edges>
    ComputerNode[] pairs = {
        a, b, e, h, j, k, a, d, f, g, j, l, b, c, g, h,
        l, m, c, h, g, i, m, n, d, e, h, i, m, o, d, f,
        h, j, n, o, o, p, i, j, e, f, i, p
    };
    for (int ii = 0; ii < pairs.length; ii += 2) {
      test.addEdge(pairs[ii], pairs[ii + 1]);
    }
    //endregion

    //region <Core>
    ArrayList<ComputerNode> coreTest = new ArrayList<>();
    coreTest.add(h);
    coreTest.add(g);
    coreTest.add(i);
    //endregion

    //region <Pre-Proccessing>
    ManualGraphCore testWithCore = new ManualGraphCore(test, coreTest);
    RoutingGraphBuilder testBuilder = new RoutingGraphBuilder(testWithCore);
    testBuilder.process(true);
    //endregion

    //region <Algorithm>
    RoutingProcedure rp = RoutingProcedure.getInstance();

    rp.setNetwork(test);
    ComputerNode[] s = {a, e, e, p, g, o};
    ComputerNode[] t = {c, a, f, k, j, l};
    int[] lengths =    {2, 2, 1, 3, 2, 2};

    rp.expVarWithHandshakes(s, t, lengths, true);
    //endregion
  }
}