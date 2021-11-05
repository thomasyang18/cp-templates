import java.io.*;
import java.util.*;
import static java.lang.Math.*;

public class Main{
	public static void main(String[] args) throws IOException {
		// br = new BufferedReader(new FileReader(".in"));
		// out = new PrintWriter(new FileWriter(".out"));
		//new Thread(null, new (), "peepee", 1<<28).start();
		
		out.close();
	}
	
	// Bare bones competitive programming template
	
	static BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
	static PrintWriter out = new PrintWriter(new BufferedOutputStream(System.out));
	static StringTokenizer st = new StringTokenizer("");
	static String read() throws IOException{return st.hasMoreTokens() ? st.nextToken():(st = new StringTokenizer(br.readLine())).nextToken();}
	static int readInt() throws IOException{return Integer.parseInt(read());}
	static long readLong() throws IOException{return Long.parseLong(read());}
	static double readDouble() throws IOException{return Double.parseDouble(read());}
}