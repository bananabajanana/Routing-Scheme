import java.io.ByteArrayInputStream;
import java.io.File;
import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.util.ArrayList;
import java.util.Random;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import org.jgrapht.Graph;
import org.jgrapht.alg.interfaces.ShortestPathAlgorithm;
import org.jgrapht.alg.shortestpath.BFSShortestPath;
import org.jgrapht.graph.DefaultEdge;
import org.jgrapht.graph.DefaultUndirectedGraph;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;
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
    /*
    try {
      fileTest();
    } catch (ParserConfigurationException e) {
      throw new RuntimeException(e);
    } catch (IOException e) {
      throw new RuntimeException(e);
    } catch (SAXException e) {
      throw new RuntimeException(e);
    }
    */
  }

  /**
   * Demo of testing on a random graph.
   */
  public static void rplgTest() {
    RoutingProcedure rp = RoutingProcedure.getInstance();

    Random rand = new Random();
    int n = 10000;
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

    rp.expVarWithHandshakes(s, t, spLength, false);
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

  public static void fileTest() throws ParserConfigurationException, IOException, SAXException {
    DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
    DocumentBuilder builder = factory.newDocumentBuilder();

    File file = new File("Routing-Scheme-Simulation/src/testGraphs/graphTemp/N_10000_beta_2.5_08.xml");
    Document doc = builder.parse(file);

    Element root = doc.getDocumentElement();

    Graph<ComputerNode, DefaultEdge> test = new DefaultUndirectedGraph<>(DefaultEdge.class);

    //region <Load Graph from File>
    NodeList nodes = ((Element)root.getElementsByTagName("snapshot").item(0))
        .getElementsByTagName("node");
    ComputerNode[] nodesArr = new ComputerNode[nodes.getLength()];
    System.out.println("Loading Nodes: " + nodes.getLength() + "\n");
    for(int i = 0; i < nodes.getLength(); i++) {
      Node nNode = nodes.item(i);
      Element eNode = (Element)nNode;

      ComputerNode a = new ComputerNode(Integer.parseInt(eNode.getAttribute("ID")));
      test.addVertex(a);
      nodesArr[Integer.parseInt(eNode.getAttribute("ID"))] = a;
      System.out.print("|");
    }

    System.out.println("Finished Loading Nodes\n");
    NodeList edges = ((Element)root.getElementsByTagName("snapshot").item(0))
        .getElementsByTagName("edge");
    System.out.println("Loading Edges: " + edges.getLength() + "\n");
    for(int i = 0; i < edges.getLength(); i++) {
      Node nEdge = edges.item(i);
      Element eEdge = (Element)nEdge;

      test.addEdge(
          nodesArr[Integer.parseInt(eEdge.getAttribute("nodeID_1"))],
          nodesArr[Integer.parseInt(eEdge.getAttribute("nodeID_2"))]
      );
      System.out.print("|");
    }
    System.out.println("Finished Loading Edges\n");
    //endregion

    ArrayList<ComputerNode> coreTest = new ArrayList<>();

    double tau = 2.5;
    double gama = ((tau - 2) / ((2 * tau) - 3)) + (1E-12);
    double gamaPrime = (1 - gama) / (tau - 1);
    double coreDegreeThreshold = Math.pow(10000, gamaPrime) / 4;
    System.out.println("Threshold: " + coreDegreeThreshold);

    //TODO: core calculation seems wrong... for some reason every node has
    //      either 4900 tbl lines or 1...
    System.out.println("Loading Core\n");
    for(ComputerNode node : test.vertexSet()) {
      if(node.getDegree() > coreDegreeThreshold) {
        coreTest.add(node);
      }
      System.out.print("|");
    }
    System.out.println("Finished Loading Core\n");

    //region <Pre-Proccessing>
    ManualGraphCore testWithCore = new ManualGraphCore(test, coreTest);
    RoutingGraphBuilder testBuilder = new RoutingGraphBuilder(testWithCore);
    testBuilder.process(true);
    //endregion


  }
}