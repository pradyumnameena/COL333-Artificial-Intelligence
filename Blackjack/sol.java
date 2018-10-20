import java.util.*;
import java.io.*;

public class sol{
	public static void main(String[] args){
		ArrayList<String> list = new ArrayList<String>();
		float p = Float.parseFloat(args[0]);
		float q = (1-p)/9;
		
		String s = "HHHHHHHHHH";
		for(int i = 0;i<33;i++){
			list.add(s);
		}
		outGen(list);
	}

	public static void outGen(ArrayList<String> list){
		File out = new File("./Policy.txt");
		try{
			BufferedWriter bw = new BufferedWriter(new FileWriter(out));
			String s = "";
			int i,j,k;
			for(i = 5;i<20;i++){
				bw.write(Integer.toString(i));
				bw.write("\t");
				s = list.get(i-5);
				for(j = 0;j<s.length();j++){
					bw.write(s.charAt(j));
					if(j==s.length()-1){
						bw.write("\n");
					}else{
						bw.write(" ");
					}
				}
			}

			for(i = 2;i<10;i++){
				bw.write("A");
				bw.write(Integer.toString(i));
				bw.write("\t");
				s = list.get(i+13);
				for(j = 0;j<s.length();j++){
					bw.write(s.charAt(j));
					if(j==s.length()-1){
						bw.write("\n");
					}else{
						bw.write(" ");
					}
				}
			}

			for(i = 2;i<11;i++){
				bw.write(Integer.toString(i));
				bw.write(Integer.toString(i));
				bw.write("\t");
				s = list.get(i+21);
				for(j = 0;j<s.length();j++){
					bw.write(s.charAt(j));
					if(j==s.length()-1){
						bw.write("\n");
					}else{
						bw.write(" ");
					}
				}
			}

			bw.write("AA");
			bw.write("\t");
			s = list.get(32);
			for(j = 0;j<s.length();j++){
				bw.write(s.charAt(j));
				if(j!=s.length()-1){
					bw.write(" ");
				}
			}
		bw.close();
		}catch(IOException ioe){

		}
	}

	public static float exward(String pla,String dea,int bet,float p,float q){
		float rval = 0;
		float[] arr = new float[21-value(dea,false)];

		for(int i = 0;i<arr.length;i++){
			arr[i] = -1;
		}

		if(dea.charAt(0)=='A'){
			arr[arr.length-1] = p*reward(pla,"AK",bet);
		}else if(dea.charAt(0)=='X' || dea.charAt(0)=='K' || dea.charAt(0)=='Q' || dea.charAt(0)=='J'){
			arr[arr.length-1] = q*reward(pla,"XA",bet);
		}else{
			arr[arr.length-1] = 0;
		}

		for(int j = arr.length-2;j>=arr.length-6;j--){
			arr[j] = reward2(pla,j+value(dea,true)+2,bet);
		}

		rval = helper(pla,dea,bet,p,q,arr);
		return rval;
	}

	public static float helper(String pla,String dea,int bet,float p,float q,float[] arr){
		float rval = 0;
		int idx = 0;
		int val = value(dea,true);
		int val2 = val;
		
		for(int i = arr.length-7;i>=0;i--){
			val = val2+i+2;
			idx = 2;
			while(idx<=11){
				if(val+idx>21){
					if(idx==10){
						rval+=(p*bet);
					}else{
						rval+=(q*bet);
					}
				}else{
					if(idx==10){
						rval+=(p*arr[i+idx]);
					}else{
						rval+=(q*arr[i+idx]);
					}
				}
				idx++;
			}
			arr[i] = rval;
		}

		rval = 0;
		for(idx=0;idx<8;idx++){
			rval+=(q*arr[idx]);
		}
		rval+=(p*(arr[8]));
		rval+=arr[arr.length-1];

		return rval;
	}

	public static float reward(String pla,String dea,int bet){
		float rval = 0;
		int pval = value(pla,true);
		int dval = value(dea,true);

		if(isBJ(pla) && isBJ(dea)){
			rval = 0;
		}else if(isBJ(pla)){
			rval = (3*bet)/2;
		}else if(pval>21){
			rval = -1*bet;
		}else if(dval>21){
			rval = bet;
		}else if(dval>pval){
			rval = -1*bet;
		}else if(pval>dval){
			rval = bet;
		}else if(isBJ(dea) && pval==21){
			rval = -1*bet;
		}else if(pval==dval){
			rval = 0;
		}
		return rval;
	}

	public static float reward2(String pla,int dea,int bet){
		float rval = 0;
		int pval = value(pla,true);
		int dval = dea;
		if(isBJ(pla)){
			rval = (3*bet)/2;
		}else if(pval>21){
			rval = -1*bet;
		}else if(dval>21){
			rval = bet;
		}else if(dval>pval){
			rval = -1*bet;
		}else if(pval>dval){
			rval = bet;
		}else if(pval==dval){
			rval = 0;
		}
		return rval;
	}

	public static int value(String hand,boolean soft){
		int rv = 0;
		int numA = 0;
		for(int i = 0;i<hand.length();i++){
			if(hand.charAt(i)=='A'){
				numA++;
			}else if(hand.charAt(i)=='K' || hand.charAt(i)=='Q' || hand.charAt(i)=='J' || hand.charAt(i)=='X'){
				rv+=10;
			}else{
				rv+=(hand.charAt(i)-'0');
			}
		}

		if(numA==0){
			return rv;
		}

		if(soft){
			rv+=(11+numA-1);
		}else{
			if(rv>10){
				rv+=numA;
			}else{
				rv+=(numA-1);
				if(rv>10){
					rv+=1;
				}else{
					rv+=11;
				}
			}
		}
		return rv;
	}

	public static boolean isBJ(String s){
		if(s.length()!=2){
			return false;
		}else{
			if(s.charAt(1)=='A' && (s.charAt(0)=='K' || s.charAt(0)=='Q' || s.charAt(0)=='J' || s.charAt(0)=='X')){
				return true;
			}else if(s.charAt(0)=='A' && (s.charAt(1)=='K' || s.charAt(1)=='Q' || s.charAt(1)=='J' || s.charAt(1)=='X')){
				return true;
			}else{
				return false;
			}
		}
	}
}