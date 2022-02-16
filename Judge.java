import java.io.*;
import java.util.*;
import static java.lang.Math.*;

public class Judge{
	
	public Judge(String prefix, String suffix, String inFile, String outFile, int numTests) throws IOException{
		outer: for (int iter =1 ; iter <= numTests; iter++) {
			String f = prefix + iter + suffix + inFile;
			String output = "output.txt";
			
			//C solver = new C(f, output); //write solver to output file
			
			String r = prefix + iter + suffix + outFile;
			
			// compare output and r
			String s1, s2;
			BufferedReader br1 = new BufferedReader(new FileReader(output));
			BufferedReader br2 = new BufferedReader(new FileReader(r));
			
			while ((s1 = br1.readLine()) != null && (s2 = br2.readLine()) != null) {
				if (s1.equals(s2)) 
				{
					//System.out.println(s1);
					//System.out.println(s2);
					//System.out.println("---------------------------");
				}
				else {
					System.out.println("WA " + iter);
					System.out.println("Output: " + s1);
					System.out.println("Expected: " + s2);
					break outer;
				}
			}
			br1.close();
			br2.close();
		}
		
		
	}
	
	public static void main(String[] args) throws IOException{
		Judge judge = new Judge("gnyr2019c/c00", "", ".in", ".ans", 5);
	}
	
	/* If on linux (or download WGET for windows) this command into terminal downloads past GNYR things
	 * wget -r -np -nH --cut-dirs=3 -R index.html http://hostname/aaa/bbb/ccc/ddd/ -P /path/to/folder
	 * Reference: https://stackoverflow.com/questions/23446635/how-to-download-http-directory-with-all-files-and-sub-directories-as-they-appear
	 */
	
}
