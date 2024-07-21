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

/**
 * Temporary Main class.
 * Includes the main function that will run on compilation.
 */
public class Main {

  // utils
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

    Toolkit.getDefaultToolkit().beep();
    try {
//      fileTest();
      PrintStream stdout = System.out;
      Date d1, d2 = new Date();
      int nodesNum = 100000;

      double[] taus = {2.9, 2.8, 2.7, 2.6, 2.5, 2.4, 2.3, 2.2, 2.1};
//      for(int i = 0; i < taus.length; i++) {
//        System.out.println("Working on t=" + taus[i] + "...");
//        d1 = new Date();
//        System.out.println("Time of Start :" + d1);
//        long diffMill = Math.abs(d1.getTime() - d2.getTime());
//        System.out.println("Time difference in seconds :" + 0.001 * diffMill);
//        d2 = d1;
//
////        double gama = ((taus[i] - 2) / ((2 * taus[i]) - 3)) + (1E-12);
////        long gamaCeil = (long)Math.ceil(Math.pow(nodesNum, gama));
//
////        multipleFileTestsManualCoreSize(nodesNum, taus[i], 1000, gamaCeil);
//        //multipleFileTests(taus[i], 2000, Integer.parseInt(args[0]));
//        multipleFileTests(taus[i], 2000, nodesNum);
//        System.setOut(stdout);
//      }

        System.out.println("Working on t=" + taus[Integer.parseInt(args[0])] + "...");
        d1 = new Date();
        System.out.println("Time of Start :" + d1);
        long diffMill = Math.abs(d1.getTime() - d2.getTime());
        System.out.println("Time difference in seconds :" + 0.001 * diffMill);
        d2 = d1;

//        double gama = ((taus[i] - 2) / ((2 * taus[i]) - 3)) + (1E-12);
//        long gamaCeil = (long)Math.ceil(Math.pow(nodesNum, gama));

//        multipleFileTestsManualCoreSize(nodesNum, taus[i], 1000, gamaCeil);
        //multipleFileTests(taus[i], 2000, Integer.parseInt(args[0]));
        Utils.multipleFileTests(taus[Integer.parseInt(args[0])], 2000, nodesNum);
        System.setOut(stdout);

    } catch (ParserConfigurationException e) {
      throw new RuntimeException(e);
    } catch (IOException e) {
      throw new RuntimeException(e);
    } catch (SAXException e) {
      throw new RuntimeException(e);
    }

    System.out.println("Done!");
    Toolkit.getDefaultToolkit().beep();
  }

 }