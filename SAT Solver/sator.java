import java.util.*;
import java.io.*;

public class sator{
	public static void main(String args[]){
		// FileInputStream g = null;
		File file = new File("./test.graph");
		// File file2 = new File("./test.satinput");
		// file2.getParentFile().mkdirs();
		// FileReader fr = new FileReader(args[0] + ".graph");
		try{
			Scanner scn = new Scanner(file);
			int n = scn.nextInt();
			int e = scn.nextInt();
			int k = scn.nextInt();

			boolean[][] arr = new boolean[n+1][n+1];
			int idx = 0;
			int i = 0;
			int j = 0;
			for(idx = 0;idx<e;idx++){
				i = scn.nextInt();
				j = scn.nextInt();
				arr[i][j] = true;
				arr[i][i] = true;
				arr[j][j] = true;
				arr[j][i] = true;
			}
			satgen(arr,k);
		}catch(FileNotFoundException ex){
			// exception
		}	
	}

	public static void satgen(boolean[][] arr,int k){
		File out = new File("test.satinput");
		try{
			BufferedWriter bw = new BufferedWriter(new FileWriter(out));
			bw.write("p cnf ");
			int num_v = arr.length-1;
			int numV = (num_v*num_v)*(2*k+1)+(num_v*k);
			bw.write(Integer.toString(numV));
			bw.write(" ");
			int numC = (num_v*num_v)*(11*k+2) + (num_v);
			bw.write(Integer.toString(numC)); 
			bw.write("\n");
			int counter = 1;
			int i = 0;
			int j = 0;
			int subidx = 0;
			int idx = 0;

			// adding edges to the file x(i,j)
			for(i = 1;i<arr.length;i++){
				for(j = 1;j<arr.length;j++){
					if(arr[i][j]){
						bw.write(Integer.toString(counter));
						bw.write(" 0");
						counter++;
						bw.write("\n");
					}else{
						bw.write(Integer.toString(-1*counter));
						bw.write(" 0");
						counter++;
						bw.write("\n");
					}
				}
			}
			// counter = n*n +1

			// for ensuring each node is present somewhere
			int[][] pointsubg = new int[arr.length-1][k];
			for(i = 0;i<pointsubg.length;i++){
				for(j = 0;j<k;j++){
					pointsubg[i][j] = counter;
					bw.write(Integer.toString(counter));
					bw.write(" ");
					counter++;
				}
				bw.write("0");
				bw.write("\n");
			}
			// counter = n*n + n*k +1

			// for pairwise presence of vertices in kth subgraph
			for(subidx = 0;subidx<k;subidx++){
				for(i = 0;i<pointsubg.length;i++){
					for(j = 0;j<pointsubg.length;j++){
						bw.write(Integer.toString(-1 * pointsubg[i][subidx]));
						bw.write(" ");
						bw.write(Integer.toString(-1 * pointsubg[j][subidx]));
						bw.write(" ");
						bw.write(Integer.toString(counter));
						bw.write(" ");
						bw.write("0");
						bw.write("\n");

						bw.write(Integer.toString(-1*counter));
						bw.write(" ");
						bw.write(Integer.toString(pointsubg[i][subidx]));
						bw.write(" ");
						bw.write("0");
						bw.write("\n");

						bw.write(Integer.toString(-1*counter));
						bw.write(" ");
						bw.write(Integer.toString(pointsubg[j][subidx]));
						bw.write(" ");
						bw.write("0");
						bw.write("\n");
						counter++;
					}
				}
			}
			// counter = n*n + n*k + k*n*n +1

			// ensuring each edge is present somewhere in the subgraphs
			for(i = 1;i<arr.length;i++){
				for(j = 1;j<arr.length;j++){
					idx = (i-1)*(arr.length-1) + j;
					bw.write(Integer.toString(-1*idx));
					bw.write(" ");
					for(subidx = 0;subidx<k;subidx++){
						bw.write(Integer.toString(counter));
						bw.write(" ");
						counter++;
					}
					bw.write("0");
					bw.write("\n");
				}
			}
			// counter-=1;
			// counter = n*n + n*k + k*n*n + k*n*n
			int numedge = 0;
			int numedgesubg = 0;
			int pairpsubg = 0;
			// main rule of the assignment
			for(subidx = 0;subidx<k;subidx++){
				for(i = 1;i<arr.length;i++){
					for(j = 1;j<arr.length;j++){
						numedge = (i-1)*(arr.length-1) + j-1;
						pairpsubg = (arr.length-1)*(arr.length+k-1) + subidx*(arr.length-1)*(arr.length-1) + numedge+1;
						numedgesubg = (arr.length-1)*(arr.length-1)*(k+1) + (k*(arr.length-1)) + subidx+1 + k*numedge;
						bw.write(Integer.toString(-1*pairpsubg));
						bw.write(" ");
						bw.write(Integer.toString(numedge+1));
						bw.write(" ");
						bw.write("0");
						bw.write("\n");

						bw.write(Integer.toString(-1*pairpsubg));
						bw.write(" ");
						bw.write(Integer.toString(numedgesubg));
						bw.write(" ");
						bw.write("0");
						bw.write("\n");

						bw.write(Integer.toString(-1*numedge-1));
						bw.write(" ");
						bw.write(Integer.toString(-1*numedgesubg));
						bw.write(" ");
						bw.write(Integer.toString(pairpsubg));
						bw.write(" ");
						bw.write("0");
						bw.write("\n");
					}
				}
			}

			// now make sure ki if e(i,j,k) then Q(i,k) as well as Q(j,k)
			int nesubg_edge = (arr.length-1)*(arr.length-1)*(k+1) + (k*(arr.length-1));
			int nesubg = 0;
			int pk1 = 0;
			int pk2 = 0;
			for(subidx = 0;subidx<k;subidx++){
				for(i = 1;i<arr.length;i++){
					for(j = 1;j<arr.length;j++){
						nesubg = k*((i-1)*(arr.length-1) + j-1) + subidx+1;
						nesubg+=nesubg_edge;
						pk1 = (arr.length-1)*(arr.length-1) + (i-1)*(k) + subidx+1;
						pk2 = (arr.length-1)*(arr.length-1) + (j-1)*(k) + subidx+1;

						bw.write(Integer.toString(-1*nesubg));
						bw.write(" ");
						bw.write(Integer.toString(pk1));
						bw.write(" ");
						bw.write("0");
						bw.write("\n");

						bw.write(Integer.toString(-1*nesubg));
						bw.write(" ");
						bw.write(Integer.toString(pk2));
						bw.write(" ");
						bw.write("0");
						bw.write("\n");

						bw.write(Integer.toString(-1*pk1));
						bw.write(" ");
						bw.write(Integer.toString(-1*pk2));
						bw.write(" ");
						bw.write(Integer.toString(nesubg));
						bw.write(" ");
						bw.write("0");
						bw.write("\n");
					}
				}
			}

			// now make sure if e(i,j) is present in kth subgraph then e(j,i) is also present
			int numedge1 = 0;
			int numedge2 = 0;
			int edge = (arr.length-1)*(arr.length-1)*(k+1) + (k*(arr.length-1));
			for(subidx = 0;subidx<k;subidx++){
				for(i = 1;i<arr.length;i++){
					for(j = 1;j<arr.length;j++){
						numedge1 = k*((i-1)*(arr.length-1) + j-1) + subidx+1;
						numedge2 = k*((j-1)*(arr.length-1) + i-1) + subidx+1;
						numedge1+=edge;
						numedge2+=edge;

						bw.write(Integer.toString(-1*numedge1));
						bw.write(" ");
						bw.write(Integer.toString(numedge2));
						bw.write(" ");
						bw.write("0");
						bw.write("\n");

						bw.write(Integer.toString(-1*numedge2));
						bw.write(" ");
						bw.write(Integer.toString(numedge1));
						bw.write(" ");
						bw.write("0");
						bw.write("\n");
					}
				}
			}
			bw.close();
		}catch(IOException ioe){
			// exception
		}
	}
}