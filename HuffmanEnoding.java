import java.io.*;
import java.util.*;
import java.util.zip.*;

class ioCheck {

	static String fileName;
	static String codes[] = new String[97];
	static String path = "";

	public static void main(String args[]){

		//nodes for all printable characters.
		Node a[] = new Node[97];
		//ASCII of printable chars from 32 to 126
		for(int i = 32;i<=126;i++){
			a[i-32] = new Node(""+(char)(i));
		}
		a[95] = new Node(""+(char)(10));//Line feed
		a[96] = new Node(""+(char)(9));//Horizontal tab
		
		Scanner s = new Scanner(System.in);
		System.out.println("Enter file name:");
		fileName = s.next();
		//Reading character by character from file
		try{
			BufferedReader in= new BufferedReader(new InputStreamReader
			(new FileInputStream(new File(fileName).getAbsoluteFile())));
			int c;
			while((c=in.read())!=-1){
				if(c==10)	a[95].fq++;
				else if(c==9)	a[96].fq++;
				else	a[c-32].fq++;//ASCII 'firstPrintableCharacter' - 32
				//System.out.println("c: " +c);
			}
		}//freq has the frequency of all letters
			
		catch(Exception e){
			System.out.println(e);
		}

		//root of the constructed tree
		Node root = makeTree(a);
		
		assignCodes(root,"");
		//System.out.println(Arrays.toString(codes));	
		
		makeCompressedFile();	
	}


	public static Node makeTree(Node a[]){
		
		//sorts the array according to frequencies, to pick the least two fqs
		Arrays.sort(a, new Comparator<Node>() {
    			public int compare(Node a, Node b) {
       				return Integer.compare(a.fq, b.fq);
    			}
		});

		int i;Node root = null;
		//At the end of the loop, value of i is equal to the first non zero index
		// of array.
		for(i = 0;a[i].fq==0;i++);
		if(i<a.length-1){
			root = new Node(a[i].data+a[i+1].data,a[i].fq+a[i+1].fq);
			root.left = new Node(a[i].data,a[i].fq,a[i].left,a[i].right);
			root.right = new Node(a[i+1].data,a[i+1].fq,a[i+1].left,a[i+1].right);
			//System.out.print(root.data+" ");
			//System.out.println(root.fq);
			a[i] = root;
			a[i+1].fq = 0;
		}

		else if(i==a.length-1){
			return a[i];
		}

		//null is returned, when a file is empty! 
		else 
			return root;
		
		return makeTree(a);
	}
	
	public static void assignCodes(Node root,String lr){
		//lr - left or right
		path += lr;
		
		//since it is a  full tree either 2 children or no children
		//checking for leaf node
		if(root.left == null){
			int temp = (root.data).charAt(0);
			if(temp == 10)	codes[95] = path;
			else if(temp == 9)	codes[96] = path;
			else codes[temp-32] = path;
		}
		//traverse till u reach leaf node
		else{
			assignCodes(root.left,"0");
			assignCodes(root.right,"1");
		}

		//on completion of left and right on a node or on reaching a leaf node 
		if(path.length()>0)
			path = path.substring(0,path.length()-1);
	}

	static void makeCompressedFile(){

		try{
			BufferedReader in = new BufferedReader(new InputStreamReader
			(new FileInputStream(new File(fileName).getAbsoluteFile())));
			//BufferedWriter writer = new BufferedWriter
			//(new FileWriter("compressed1"));
			PrintWriter writer = new PrintWriter("Encoded.txt", "UTF-8");
/*			DataOutputStream out = new DataOutputStream(
        	new FileOutputStream("compressed"));
			BufferedOutputStream out = new BufferedOutputStream(
new GZIPOutputStream(
new FileOutputStream("test.gz")));*/
			int c;String print;
			while((c=in.read())!=-1){
				if(c==10)	print = codes[95];
				else if(c==9)	print = codes[96];
				else	print = codes[c-32];//ASCII 'a' - 97
				writer.println(print);
			}
			writer.close();
		}//freq has the frequency of all letters
			
		catch(Exception e){
			System.out.println(e);
		}
	}
}

class Node{
	String data;
	int fq;
	Node left;
	Node right;
	public Node(String d,int f,Node l,Node r){
		data = d;
		fq = f;
		left = l;
		right = r;
	}
	public Node(String d,int f){
		data = d;
		fq = f;
	}
	public Node(int f){
		fq = f;
	}
	public Node(String d){
		data = d;
	}
}

