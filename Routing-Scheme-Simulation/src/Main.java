import java.io.ByteArrayInputStream;
import java.io.File;
import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.List;
import java.util.Random;
import java.util.Set;
import java.util.stream.Collectors;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import org.jgrapht.Graph;
import org.jgrapht.GraphPath;
import org.jgrapht.Graphs;
import org.jgrapht.alg.connectivity.ConnectivityInspector;
import org.jgrapht.alg.interfaces.ShortestPathAlgorithm;
import org.jgrapht.alg.shortestpath.BFSShortestPath;
import org.jgrapht.graph.DefaultEdge;
import org.jgrapht.graph.DefaultUndirectedGraph;
import org.jgrapht.graph.SimpleGraph;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;
import simulator.ComputerNode;
import simulator.RoutingGraphBuilder;
import simulator.RoutingGraphBuilder.Print;
import simulator.graphs.AdaptedFdrg;
import simulator.graphs.GraphWithCore;
import simulator.graphs.ManualGraphCore;

/**
 * Temporary Main class.
 * Includes the main function that will run on compilation.
 */
public class Main {

  public static void printAllVertexesDeg(Graph<ComputerNode, DefaultEdge> graph){
    System.out.println("index , degree");
    for(ComputerNode node : graph.vertexSet()){
      System.out.println( node.getNodeIndex() + "," + node.getDegree());
    }
  }

  /**
   * Demo of the tests and analysis.

   * @param args noop
   */
  public static void main(String[] args) {
//    rplgTest();
//    manualTest2();
//    manualTest();

    try {
//      fileTest();
      multipleFileTests(2.1, 1000);
    } catch (ParserConfigurationException e) {
      throw new RuntimeException(e);
    } catch (IOException e) {
      throw new RuntimeException(e);
    } catch (SAXException e) {
      throw new RuntimeException(e);
    }
  }

  /**
   * Demo of testing on a random graph.
   */
  public static void rplgTest() {
    RoutingProcedure rp = RoutingProcedure.getInstance();

    Random rand = new Random();
    int n = 10000;
    float tau = 2.3f;
    int sampleSize = 2500;
    AdaptedFdrg testGraph = new AdaptedFdrg(n, tau);

    RoutingGraphBuilder testBuilder = new RoutingGraphBuilder(testGraph);
    testBuilder.process(Print.Detailed);
    printAllVertexesDeg(testGraph.getGraph());
    ///HELPER DATA PRINTING
    System.out.println("\n\n~~~~~USEFUL-DATA~~~~~~\n");
    testGraph.temporaryPrintThreshold();
    System.out.println("Landmarks: " + testGraph.getCore().size());
    ///END

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

    rp.expVarNoHandshakes(s, t, spLength, Print.Detailed);
  }


  /**
   * Demo of testing on a specific given graph.
   */
  public static void manualTest2() {

    Graph<ComputerNode, DefaultEdge> test = new DefaultUndirectedGraph<>(DefaultEdge.class);

    for(int i = 0; i < 1000; i++) {
      test.addVertex((new ComputerNode(i)));
    }
    Object[] arr = test.vertexSet().toArray();

    for(int i = 0; i < 1000; i++) {
      test.addEdge((ComputerNode) arr[i%1000], (ComputerNode) arr[(i+1)%1000]);
    }
    ArrayList<ComputerNode> coreTest = new ArrayList<>();
    for(int i = 0; i < 1000; i+=100) {
      coreTest.add((ComputerNode) arr[i]);
    }

//
//    //region <Vertices>
//    ComputerNode a = new ComputerNode(1);
//    test.addVertex(a);
//    ComputerNode c = new ComputerNode(3);
//    test.addVertex(c);
//    ComputerNode b = new ComputerNode(2);
//    test.addVertex(b);
//    ComputerNode d = new ComputerNode(4);
//    test.addVertex(d);
//    ComputerNode e = new ComputerNode(5);
//    test.addVertex(e);
//    ComputerNode f = new ComputerNode(6);
//    test.addVertex(f);
//    ComputerNode g = new ComputerNode(7);
//    test.addVertex(g);
//    ComputerNode h = new ComputerNode(8);
//    test.addVertex(h);
//    ComputerNode i = new ComputerNode(9);
//    test.addVertex(i);
//    ComputerNode j = new ComputerNode(10);
//    test.addVertex(j);
//    ComputerNode k = new ComputerNode(11);
//    test.addVertex(k);
//    ComputerNode l = new ComputerNode(12);
//    test.addVertex(l);
//    ComputerNode m = new ComputerNode(13);
//    test.addVertex(m);
//    ComputerNode n = new ComputerNode(14);
//    test.addVertex(n);
//    ComputerNode o = new ComputerNode(15);
//    test.addVertex(o);
//    ComputerNode p = new ComputerNode(16);
//    test.addVertex(p);
//    //endregion
//
//    //region <Edges>
//    ComputerNode[] pairs = {
//            a, b, e, h, j, k, a, d, f, g, j, l, b, c, g, h,
//            l, m, c, h, g, i, m, n, d, e, h, i, m, o, d, f,
//            h, j, n, o, o, p, i, j, e, f, i, p
//    };
//    for (int ii = 0; ii < pairs.length; ii += 2) {
//      test.addEdge(pairs[ii], pairs[ii + 1]);
//    }
//    //endregion

    //region <Core>
//    ArrayList<ComputerNode> coreTest = new ArrayList<>();
//    coreTest.add(h);
//    coreTest.add(g);
//    coreTest.add(i);
    //endregion

    //region <Pre-Proccessing>
    ManualGraphCore testWithCore2 = new ManualGraphCore(test, coreTest);
    RoutingGraphBuilder testBuilder = new RoutingGraphBuilder(testWithCore2);
    testBuilder.process(Print.Detailed);
    //endregion
    printAllVertexesDeg(testWithCore2.getGraph());
    //region <Algorithm>
    RoutingProcedure rp = RoutingProcedure.getInstance();

    rp.setNetwork(test);
//    ComputerNode[] s = {a, e, e, p, g, o};
//    ComputerNode[] t = {c, a, f, k, j, l};
//    int[] lengths =    {2, 2, 1, 3, 2, 2};
//
//    rp.expVarWithHandshakes(s, t, lengths, false);
    //endregion
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
    testBuilder.process(Print.Detailed);
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

  public static void multipleFileTests(double tau, int sampleSize)
      throws ParserConfigurationException, IOException, SAXException {
    DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
    DocumentBuilder builder = factory.newDocumentBuilder();

    int fileAmount = 30;
    int nodesNum = 10000;

    for(int i = 1; i <= fileAmount; i++) {
      System.out.print("\n" + i + "/" + fileAmount);
      String path = "Routing-Scheme-Simulation/src/testGraphs/graphTemp/N_"
          + nodesNum + "_beta_" + tau + "_" + String.format("%02d", i) + ".xml";
      fileTest(nodesNum, tau, sampleSize, path, Print.Answer, Print.Answer, false);
    }

  }


  public static ArrayList<ComputerNode> practicalCoreSelection(double tau, int nodesNum, Graph<ComputerNode, DefaultEdge> graph) {
    ArrayList<ComputerNode> coreTest;

    double gama = ((tau - 2) / ((2 * tau) - 3)) + (1E-12);
    long gamaCeil = (long)Math.ceil(Math.pow(nodesNum, gama));

    List<ComputerNode> coreNodes = graph.vertexSet().stream()
        .sorted((v1, v2) -> Integer.compare(graph.degreeOf(v2), graph.degreeOf(v1)))
        .limit(gamaCeil)
        .collect(Collectors.toList());

    coreTest = new ArrayList<>(coreNodes);

    return coreTest;
  }

  public static ArrayList<ComputerNode> theoreticalCoreSelection(double tau, int nodesNum, Graph<ComputerNode, DefaultEdge> graph) {
    ArrayList<ComputerNode> coreTest = new ArrayList<>();

    double gama = ((tau - 2) / ((2 * tau) - 3)) + (1E-12);
    double gamaPrime = (1 - gama) / (tau - 1);
    double coreDegreeThreshold = Math.pow(nodesNum, gamaPrime) / 4;

    for(ComputerNode node : graph.vertexSet()) {
      if(graph.edgesOf(node).size() > coreDegreeThreshold) {
        coreTest.add(node);
      }
    }
    return coreTest;
  }

  public static void fileTest(int nodesNum, double tau, int sampleSize, String path, Print printTbls, Print printDists, boolean printDegs) throws ParserConfigurationException, IOException, SAXException {
    DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
    DocumentBuilder builder = factory.newDocumentBuilder();
    RoutingProcedure rp = RoutingProcedure.getInstance();
    System.out.print("\n");

    File file = new File(path);
    Document doc = builder.parse(file);

    Element root = doc.getDocumentElement();

    Graph<ComputerNode, DefaultEdge> test = new DefaultUndirectedGraph<>(DefaultEdge.class);

    //region <Load Graph from File>
    NodeList nodes = ((Element)root.getElementsByTagName("snapshot").item(0))
        .getElementsByTagName("node");
    ComputerNode[] nodesArr = new ComputerNode[nodes.getLength()];
    for(int i = 0; i < nodes.getLength(); i++) {
      Node nNode = nodes.item(i);
      Element eNode = (Element)nNode;

      ComputerNode a = new ComputerNode(Integer.parseInt(eNode.getAttribute("ID")));
      test.addVertex(a);
      nodesArr[Integer.parseInt(eNode.getAttribute("ID"))] = a;
    }

    System.out.print(".");
    NodeList edges = ((Element)root.getElementsByTagName("snapshot").item(0))
        .getElementsByTagName("edge");
    for(int i = 0; i < edges.getLength(); i++) {
      Node nEdge = edges.item(i);
      Element eEdge = (Element)nEdge;

      test.addEdge(
          nodesArr[Integer.parseInt(eEdge.getAttribute("nodeID_1"))],
          nodesArr[Integer.parseInt(eEdge.getAttribute("nodeID_2"))]
      );
    }
    System.out.print(".");
    //endregion

    //region Filtering LCC
    ConnectivityInspector<ComputerNode, DefaultEdge> inspector = new ConnectivityInspector<>(test);
    List<Set<ComputerNode>> connectedComponents = inspector.connectedSets();

    Set<ComputerNode> largestComponent = connectedComponents.stream()
        .max(Comparator.comparingInt(Set::size))
        .orElseThrow(() -> new IllegalStateException("There should be at least one connected component"));

    Graph<ComputerNode, DefaultEdge> LCC = new DefaultUndirectedGraph<>(DefaultEdge.class);

    Graphs.addAllVertices(LCC, largestComponent);

    for (ComputerNode vertex : largestComponent) {
      for (DefaultEdge edge : test.edgesOf(vertex)) {
        ComputerNode source = test.getEdgeSource(edge);
        ComputerNode target = test.getEdgeTarget(edge);

        if (largestComponent.contains(source) && largestComponent.contains(target)) {
          LCC.addEdge(source, target);
        }
      }
    }

    System.out.print(".");
    //endregion

    ArrayList<ComputerNode> coreTest = practicalCoreSelection(tau, nodesNum, LCC);
//    ArrayList<ComputerNode> coreTest = theoreticalCoreSelection(tau, nodesNum, LCC);
    System.out.print(".");

    System.out.println("\nNodes In LCC: " + largestComponent.size() + "\nNodes In Core: " + coreTest.size());

    if(coreTest.size() == 0) {
      return;
    }

    //region <Pre-Proccessing>
    ManualGraphCore testWithCore = new ManualGraphCore(LCC, coreTest);
    RoutingGraphBuilder testBuilder = new RoutingGraphBuilder(testWithCore);
    testBuilder.process(printTbls);
    //endregion

    //region <Stretch-Calc>
    ComputerNode[] s = new ComputerNode[sampleSize];
    ComputerNode[] t = new ComputerNode[sampleSize];
    int[] spLength = new int [sampleSize];

    Random rand = new Random();
    BFSShortestPath<ComputerNode, DefaultEdge> bfs = new BFSShortestPath<>(LCC);
    List<ComputerNode> vertices = new ArrayList<>(LCC.vertexSet());
    for (int i = 0; i < sampleSize; i++) {
      int tempS = rand.nextInt(largestComponent.size());
      int tempT = rand.nextInt(largestComponent.size());
      while(tempT == tempS) {
        tempT = rand.nextInt(largestComponent.size());
      }
      s[i] = vertices.get(tempS);
      t[i] = vertices.get(tempT);

      ShortestPathAlgorithm.SingleSourcePaths<ComputerNode, DefaultEdge> shortestPathsFromV =
          bfs.getPaths(s[i]);
      if (shortestPathsFromV.getPath(t[i]) == null) {
        i--;
      } else {
        spLength[i] = shortestPathsFromV.getPath(t[i]).getLength();
      }
    }

    rp.expVarNoHandshakes(s, t, spLength, printDists);
    //endregion

    if(printDegs) {
      printAllVertexesDeg(testWithCore.getGraph());
    }
  }
}