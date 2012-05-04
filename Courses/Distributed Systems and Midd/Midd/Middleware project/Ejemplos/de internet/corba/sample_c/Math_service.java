package corba.sample_c;
//
import java.io.*;
import java.util.*;
import My_Math.*;

//
public class Math_service extends Interface_MathPOA {
	//	
	public Math_service() {
		super();
	}
	// My_Array was defined in My_Math.idl and idlj created an type suitabel My_Array in Java
	public Result addition(
		My_Math.My_Array a,
		int line_a,
		int row_a,
		My_Math.My_Array b,
		int line_b,
		int row_b) {
		String msg = "";
		Result result = null;
		float c[][] = new float[My_Math.SIZE_MAX.value][My_Math.SIZE_MAX.value];
		// Arrays are fixed in CORBA
		// idlj creates My_Math.SIZE_MAX.value from Math.idl
		//
		try {
			if (line_a <= 0 || row_a <= 0 || line_b <= 0 || row_b <= 0)
				throw new Exception("It is impossible realize operation with arrays empty");
			if (!(line_a == line_b && row_a == row_b))
				throw new Exception("It is impossible realize the addition of arrays with different dimension");
			if (a == null || b == null)
				throw new Exception("It is impossible realize the addition with an array null");
			//
			// float c[][] = new float[line_a][row_a];  Remarks: In CORBA all arrays have fixed dimension, 
			//                                                   because this I cannot use this line of instruction
			for (int i = 0; i < line_a; i++)
				for (int j = 0; j < row_a; j++)
					c[i][j] = a.bi_array[i][j] + b.bi_array[i][j];
			//
			msg = "Addition was realized with success";
			result = new Result(c, line_a, row_a, msg, "null");
			// "null" is passed as string because null not exist in CORBA

		} catch (Exception e) {
			msg = "Addition was not realized";
			result = new Result(c, 0, 0, msg, e.toString());

		}
		return result;
	}
	//
	public String get_help() {
		String msg =
			"\n**Server Math**\nmethods:\n addition: return the addition of two arrays\n subtraction: return the subtraction of two arrays\n multiplication: return the multiplication of two arrays\n transpose: return the transpose of an array\n";
		return msg;
	}
	//
	public Result multiplication(
		My_Math.My_Array a,
		int line_a,
		int row_a,
		My_Math.My_Array b,
		int line_b,
		int row_b) {
		String msg = "";
		float c[][] = new float[My_Math.SIZE_MAX.value][My_Math.SIZE_MAX.value];
		// Arrays are fixed in CORBA
		// idlj creates My_Math.SIZE_MAX.value from Math.idl
		//

		Result result = null;
		try {
			// testa os arrays
			if (line_a <= 0 || row_a <= 0 || line_b <= 0 || row_b <= 0)
				throw new Exception("It is impossible realize operation with arrays empty");
			if (row_a != line_b)
				throw new Exception("It is impossible realize the multiplication  of arrays incompatibles");
			if (a == null || b == null)
				throw new Exception("It is impossible realize the operation with an array null");
			//
			//
			// float c[][] = new float[line_a][row_a];  Remarks: In CORBA all arrays have fixed dimension, 
			//                                                   because this I cannot use this line of instruction
			for (int i = 0; i < line_a; i++)
				for (int j = 0; j < row_b; j++)
					for (int k = 0; k < row_a; k++)
						c[i][j] = c[i][j] + a.bi_array[i][k] * b.bi_array[k][j];
			//
			msg = "Multiplication was realized with success";
			result = new Result(c, line_a, row_b, msg, "null");
			// "null" is passed as string because null not exist in CORBA
			//    
		} catch (Exception e) {
			msg = "Multiplication was not realized";
			result = new Result(c, 0, 0, msg, e.toString());
		}

		return result;
	}
	//
	public Result subtraction(
		My_Math.My_Array a,
		int line_a,
		int row_a,
		My_Math.My_Array b,
		int line_b,
		int row_b) {
		String msg = "";
		Result result = null;
		float c[][] = new float[My_Math.SIZE_MAX.value][My_Math.SIZE_MAX.value];
		// Arrays are fixed in CORBA
		// idlj creates My_Math.SIZE_MAX.value from Math.idl
		//
		try {
			// testa os arrays
			if (line_a <= 0 || row_a <= 0 || line_b <= 0 || row_b <= 0)
				throw new Exception("It is impossible realize operation with arrays empty");
			if (!(line_a == line_b && row_a == row_b))
				throw new Exception("It is impossible realize the subtraction of arrays with different dimension");
			if (a == null || b == null)
				throw new Exception("It is impossible realize the subtraction with an array null");
			//
			// float c[][] = new float[line_a][row_a];  Remarks: In CORBA all arrays have fixed dimension, 
			//                                                   because this I cannot use this line of instruction
			for (int i = 0; i < line_a; i++)
				for (int j = 0; j < row_a; j++)
					c[i][j] = a.bi_array[i][j] - b.bi_array[i][j];
			//
			msg = "Subtraction was realized with success";
			result = new Result(c, line_a, row_a, msg, "null");
			// "null" is passed as string because null not exist in CORBA

		} catch (Exception e) {
			msg = "Subtraction was not realized";
			result = new Result(c, 0, 0, msg, e.toString());

		}
		return result;

	}
	//
	public Result transpose(My_Math.My_Array a, int line_a, int row_a) {
		String msg = "";
		Result result = null;
		float c[][] = new float[My_Math.SIZE_MAX.value][My_Math.SIZE_MAX.value];
		// Arrays are fixed in CORBA
		// idlj creates My_Math.SIZE_MAX.value from Math.idl
		//
		try {
			// testa o array
			if (line_a <= 0 || row_a <= 0)
				throw new Exception("It is impossible realize operation with arrays empty");
			if (a == null)
				throw new Exception("It is impossible realize the operation with an array null");
			//
			// float c[][] = new float[line_a][row_a];  Remarks: In CORBA all arrays have fixed dimension, 
			//                                                   because this I cannot use this line of instruction
			for (int i = 0; i < line_a; i++)
				for (int j = 0; j < row_a; j++)
					c[j][i] = a.bi_array[i][j];
			//
			msg = "Transpose was realized with success";
			result = new Result(c, row_a, line_a, msg, "null");
			// "null" is passed as string because null not exist in CORBA

		} catch (Exception e) {
			msg = "Trasnpose was not realized";
			result = new Result(c, 0, 0, msg, e.toString());

		}
		return result;
	}
}
