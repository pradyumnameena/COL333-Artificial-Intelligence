import java.util.*;
import java.io.*;

public class graphgen{
	public static void main(String args[]){
		File file = new File("./test.graph");
		try{
			Scanner scn = new Scanner(file);
			int n = scn.nextInt();
			int e = scn.nextInt();
			int k = scn.nextInt();
			subggen(n,k);
		}catch(FileNotFoundException ex){
			// exception
		}	
	}

	public static void subggen(int n,int k){
		File out = new File("./test.satoutput");
		File helper = new File("./test.subgraphs");
		try{
			BufferedWriter bw1 = new BufferedWriter(new FileWriter(helper));
			BufferedReader bw2 = new BufferedReader(new FileReader(out));
			String s = bw2.readLine();
			if(s.equals("SAT")){
				s = bw2.readLine();
				String[] parts = s.split(" ");
				int idx = 0;
				int i = 0;
				int num = 0;
				int count = 0;
				for(idx = 0;idx<k;idx++){
					bw1.write("#");
					bw1.write(Integer.toString(idx+1));
					bw1.write(" ");
					s = "";
					count = 0;
					for(i = 0;i<n;i++){
						num = Integer.parseInt(parts[n*(n-1) + i*k + idx]);
						if(num>0){
							s+=(Integer.toString(i+1));
							s+=" ";
							count++;
						}
					}
					bw1.write(Integer.toString(count));
					bw1.write("\n");
					bw1.write(s.substring(0,s.length()-1));
					if(idx==k-1){

					}else{
						bw1.write("\n");
					}	
				}
				bw1.close();
				bw2.close();
			}else{
				bw1.write("0");
				bw1.close();
				bw2.close();
			}
		}catch(IOException ioe){
			// exception
		}
	}
}
