import java.util.*;
import java.io.*;

public class SAT {

	

	static BufferedWriter bw =null;
	static int[][] variable_names=null;

	static int variable_name_count=0;
	public static void Non_Empty(){
		try{
			for (int k=1;k<variable_names[0].length;k++) {
				for (int i=1;i<variable_names.length;i++ ) {
					bw.write(variable_names[i][k]+" ");
				}
				bw.write("0\n");
			}
		}catch(Exception e){
			System.out.println("Something went wrong in Non_Empty function");
		}
	}

	public static void AtLeast_OneNode(boolean[][] adjacencyMatrix,int k_subgraph){
		try{
			for (int i=1;i<variable_names.length ;i++ ) {
				for (int k=1;k<variable_names[0].length;k++) {
					bw.write(variable_names[i][k]+" ");
				}
				bw.write("0\n");
			}
		}catch(Exception e){
			System.out.println("Something went wrong in AtLeast_OneNode function");
		}
	}
	public static void AllKgraphComplete(boolean[][] adjacencyMatrix,int k_subgraph){
		try{
			for (int k=1;k<variable_names[0].length;k++) {
				for (int i=1;i<variable_names.length;i++) {
					for (int j=i+1;j<variable_names.length ;j++ ) {
						if(adjacencyMatrix[i][j]==false){
							bw.write("-"+variable_names[i][k]+" -"+variable_names[j][k]+" 0\n");
						}
					}
				}
			}
		}catch(Exception e){
			System.out.println("Something went wrong in AtLeast_OneNode function");
		}
	}

	public static void No_Subgraph(boolean[][] adjacencyMatrix){
		try{
			String s="";
			for (int k1=1;k1<variable_names[0].length;k1++) {
				for (int k2=1;k2<variable_names[0].length ;k2++ ) {
					if (k1 == k2){
						continue;
					}
					for (int i=1;i<variable_names.length;i++) {
						variable_name_count+=1;
						bw.write(variable_name_count+" ");
						s+="-"+variable_name_count+" "+variable_names[i][k1]+" 0\n";
						s+="-"+variable_name_count+" -"+variable_names[i][k2]+" 0\n";
						s+=""+variable_name_count+" -"+variable_names[i][k1]+" "+variable_names[i][k2]+" 0\n";
						// bw.write("0\n");
						// variable_name_count+=1;
						// bw.write(s);
						// s="";
						// bw.write(variable_name_count+" ");
						// s+="-"+variable_name_count+" "+variable_names[i][k2]+" 0\n";
						// s+="-"+variable_name_count+" -"+variable_names[i][k1]+" 0\n";
						// s+=""+variable_name_count+" -"+variable_names[i][k2]+" "+variable_names[i][k1]+" 0\n";

					}
					bw.write("0\n");
					bw.write(s);
					s="";
				}		
			}	
		}	
		catch(Exception e){
			System.out.println("Something went wrong in No_Subgraph function");
		}
	}
	public static void AtLeastOneEdge(boolean[][] adjacencyMatrix){
		try{
			String s="";
			for (int i=1;i<variable_names.length;i++ ) {
				for (int j=i+1;j<variable_names.length;j++) {
					if (adjacencyMatrix[i][j]==true) {
						for (int k=1;k<variable_names[0].length;k++) {
							variable_name_count+=1;
							bw.write(variable_name_count+" ");
							s+="-"+variable_name_count+" "+variable_names[i][k]+" 0\n";
							s+="-"+variable_name_count+" "+variable_names[j][k]+" 0\n";
							s+=variable_name_count+" -"+variable_names[j][k]+" -"+variable_names[i][k]+" 0\n";
						}
						bw.write("0\n");
						bw.write(s);
						s="";
					}
				}
			}	
		}catch(Exception e){
			System.out.println("Something went wrong in No_Subgraph function");
		}
	}

	public static void GenerateSAT(boolean[][] adjacencyMatrix,int k,String fileName){

		variable_names=new int[adjacencyMatrix.length][k+1];
		for (int j=0;j<variable_names.length ;j++ ) {
			variable_names[j][0]=0;
		}
		for (int j=0;j<variable_names[0].length ;j++ ) {
			variable_names[0][j]=0;
		}
		for (int i=1;i<variable_names.length;i++ ) {
			for (int j=1;j<variable_names[0].length ;j++ ) {
				variable_names[i][j]=k*(i-1)+j;
			}
		}
		variable_name_count=(variable_names.length-1)*(variable_names[0].length-1);
		File sat_input_file=new File(fileName+".satinput");
		try{
			bw = new BufferedWriter(new FileWriter(sat_input_file));
			bw.write("p cnf ");
			int num_v = adjacencyMatrix.length-1;
			int numV = (num_v*num_v)*(2*k+1)+(num_v*k);
			bw.write(Integer.toString(numV));
			bw.write(" ");
			int numC = 2*(num_v*num_v)*(4*k+1) + (num_v);
			bw.write(Integer.toString(numC)); 
			bw.write("\n");
			Non_Empty();
			No_Subgraph(adjacencyMatrix);
			AtLeast_OneNode(adjacencyMatrix,k);
			AtLeastOneEdge(adjacencyMatrix);
			AllKgraphComplete(adjacencyMatrix,k);
			bw.close();
		}
		catch(Exception e){
			System.out.println("Something went wrong! reading a file");
		}

	}



	public static void main(String[] args) {
		File file = new File(args[0]+ ".graph");
		try{
			Scanner scn = new Scanner(file);
			int n = scn.nextInt();
			int e = scn.nextInt();
			int k = scn.nextInt();

			boolean[][] adjacencyMatrix = new boolean[n+1][n+1];
			for (int i=0;i<adjacencyMatrix.length ;i++ ) {
				for (int j=0;j<adjacencyMatrix[0].length ;j++ ) {
					adjacencyMatrix[i][j]=false;
				}
			}

			int idx = 0;
			int i = 0;
			int j = 0;
			for(idx = 0;idx<e;idx++){
				i = scn.nextInt();
				j = scn.nextInt();
				adjacencyMatrix[i][j] =true;
				adjacencyMatrix[j][i] =true;
			}
			GenerateSAT(adjacencyMatrix,k,args[0]);
		}catch(FileNotFoundException ex){
			// exception
		}	

	}
}