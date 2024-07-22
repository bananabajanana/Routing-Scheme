import java.io.BufferedOutputStream;
import java.io.ByteArrayInputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.PrintStream;
import java.io.UnsupportedEncodingException;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.Date;
import java.util.List;
import java.util.Random;
import java.util.Set;
import java.util.function.BiFunction;
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
import java.awt.Toolkit;

public class Utils {

    public static void printAllVertexesDeg(Graph<ComputerNode, DefaultEdge> graph){
        System.out.println("index , degree");
        for(ComputerNode node : graph.vertexSet()){
            System.out.println( node.getNodeIndex() + "," + node.getDegree());
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

    // test files, add param graph, core and write an example
    /**
     * Demo of testing on a specific given graph.
     */
    public static void manualTest(Graph<ComputerNode, DefaultEdge> test, ArrayList<ComputerNode> coreTest,ComputerNode[] s, ComputerNode[] t, int[] lengths ) {
        //region <Pre-Proccessing>
        ManualGraphCore testWithCore = new ManualGraphCore(test, coreTest);
        RoutingGraphBuilder testBuilder = new RoutingGraphBuilder(testWithCore);
        testBuilder.process(Print.Detailed);
        //endregion

        //region <Algorithm>
        RoutingProcedure rp = RoutingProcedure.getInstance();

        rp.setNetwork(test);

        rp.expVarWithHandshakes(s, t, lengths, true);
        //endregion
    }

    public static void multipleFileTests(double tau, int sampleSize, int nodesNum)
            throws ParserConfigurationException, IOException, SAXException {
        DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
        DocumentBuilder builder = factory.newDocumentBuilder();

        int fileAmount = 30;

        System.setOut(new PrintStream(new BufferedOutputStream(new FileOutputStream("Routing-Scheme-Simulation/Outputs/p"
                + tau + "_n" + nodesNum + ".txt")), true));

//    System.out.println("nodesNum,LCCNodesNum,tau,bestThreshold");
        System.out.println("LCCNodesNum,tblSizeExp,tblSizeVar,stretchExp,stretchVar");
        for(int i = 1; i <= fileAmount; i++) {
            //System.out.print("\n" + i + "/" + fileAmount);
            String path = "Routing-Scheme-Simulation/src/testGraphs/graphTemp/N_"
                    + nodesNum + "_beta_" + tau + "_" + String.format("%02d", i) + ".xml";
            fileTest(nodesNum, tau, sampleSize, path, Print.Answer, Print.Answer, false);
//      findBarrier(nodesNum, tau, path);
        }

        //double gama = ((tau - 2) / ((2 * tau) - 3)) + (1E-12);
        //double gamaPrime = (1 - gama) / (tau - 1);
        //double coreDegreeThreshold = Math.pow(nodesNum, gamaPrime) / 4;
        double coreDegreeThreshold = (0.294 * nodesNum + 2586.3) * Math.pow(1 / (2.85765 * Math.pow(nodesNum, 0.118)), tau);
        System.out.println("\n Theoretical Threshold was " + coreDegreeThreshold);
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
    public static ArrayList<ComputerNode> practicalCoreSelection(double tau, int nodesNum, Graph<ComputerNode, DefaultEdge> graph, long coreSize) {
        ArrayList<ComputerNode> coreTest;

        List<ComputerNode> coreNodes = graph.vertexSet().stream()
                .sorted((v1, v2) -> Integer.compare(graph.degreeOf(v2), graph.degreeOf(v1)))
                .limit(coreSize)
                .collect(Collectors.toList());

        coreTest = new ArrayList<>(coreNodes);

        return coreTest;
    }

    public static ArrayList<ComputerNode> ThorupAndZwickCoreSelection(double tau, int nodesNum, Graph<ComputerNode, DefaultEdge> graph){
        BiFunction<ArrayList<ComputerNode>, Double, ArrayList<ComputerNode> > sample = (ArrayList<ComputerNode> w, Double s) -> {
            ArrayList<ComputerNode> selectedSubSet = new ArrayList<>();
            double len = w.size();
            Random random = new Random();
            final double probabilityForVertexToBeSelected = s / len;
            if(len <= s){
                return selectedSubSet;
            }
            for (ComputerNode vertex : w){
                if(random.nextDouble() < probabilityForVertexToBeSelected){
                    selectedSubSet.add(vertex);
                }
            }
            return selectedSubSet;
        };

        BFSShortestPath<ComputerNode, DefaultEdge> bfs = new BFSShortestPath<>(graph);
        BiFunction<ArrayList<ComputerNode>, ComputerNode, Integer> findClusterSizeInUndirectedGraph = (ArrayList<ComputerNode> core, ComputerNode vertexToFindCoreTo) -> {
            ShortestPathAlgorithm.SingleSourcePaths<ComputerNode, DefaultEdge> shortestPathsFromV =bfs.getPaths(vertexToFindCoreTo);
            Double distFromClosestLandmark = Double.POSITIVE_INFINITY;
            Integer clusterSize = 0;
            for (ComputerNode landmark : core){
                if (vertexToFindCoreTo == landmark){
                    return 0;
                }
                GraphPath<ComputerNode, DefaultEdge> path = shortestPathsFromV.getPath(landmark);
                if(path != null){
                    distFromClosestLandmark = distFromClosestLandmark < path.getLength() ? distFromClosestLandmark : path.getLength();
                }
            }
            for (ComputerNode node : graph.vertexSet()){
                if (node == vertexToFindCoreTo){ continue;}
                if (shortestPathsFromV.getPath(node).getLength() < distFromClosestLandmark){
                    clusterSize++;
                }
            }
            return clusterSize;
        };

        Double s = Math.sqrt((nodesNum / Math.log(nodesNum)));
        Double wThreshold = (4 * nodesNum) / s;
        ArrayList<ComputerNode> core = new ArrayList<>();
        ArrayList<ComputerNode> w = new ArrayList<ComputerNode>(graph.vertexSet());

        while (!w.isEmpty()){
            core.addAll(sample.apply(w, s));
            w.clear();
            for(ComputerNode node : w){
                if (findClusterSizeInUndirectedGraph.apply(core, node) > wThreshold){
                    w.add(node);
                }
            }
        }
        return core;
    }

    public static ArrayList<ComputerNode> theoreticalCoreSelection(double tau, int nodesNum, Graph<ComputerNode, DefaultEdge> graph) {
        ArrayList<ComputerNode> coreTest = new ArrayList<>();

        //double gama = ((tau - 2) / ((2 * tau) - 3)) + (1E-12);
        //double gamaPrime = (1 - gama) / (tau - 1);
        //double coreDegreeThreshold = Math.pow(nodesNum, gamaPrime) / 4;
        double coreDegreeThreshold = (0.294 * nodesNum + 2586.3) * Math.pow(1 / (2.85765 * Math.pow(nodesNum, 0.118)), tau);

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

        //endregion

//    ArrayList<ComputerNode> coreTest = ThorupAndZwickCoreSelection(tau, nodesNum, LCC);
//    ArrayList<ComputerNode> coreTest = practicalCoreSelection(tau, nodesNum, LCC);
        ArrayList<ComputerNode> coreTest = theoreticalCoreSelection(tau, nodesNum, LCC);

        System.out.print(largestComponent.size() + ",");

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

    public static void fileTest(int nodesNum, double tau, int sampleSize, String path, Print printTbls, Print printDists, boolean printDegs, long coreSize) throws ParserConfigurationException, IOException, SAXException {
        DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
        DocumentBuilder builder = factory.newDocumentBuilder();
        RoutingProcedure rp = RoutingProcedure.getInstance();

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

        //endregion

        ArrayList<ComputerNode> coreTest = practicalCoreSelection(tau, nodesNum, LCC, coreSize);

        System.out.print(largestComponent.size() + ",");

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

    public static void findBarrier(int nodesNum, double tau, String path) throws ParserConfigurationException, IOException, SAXException {
        DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
        DocumentBuilder builder = factory.newDocumentBuilder();
        RoutingProcedure rp = RoutingProcedure.getInstance();

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

        //endregion

        ArrayList<ComputerNode> core = practicalCoreSelection(tau, nodesNum, LCC);
        double barrier = LCC.degreeOf(core.stream().min(Comparator.comparingInt(LCC::degreeOf)).orElse(null));
        System.out.println(nodesNum + "," + LCC.vertexSet().size() + "," + tau + "," + barrier);
    }
}
