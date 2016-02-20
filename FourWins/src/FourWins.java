import java.applet.Applet;
import java.awt.Color;
import java.awt.Graphics;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.util.Random;
import java.util.Scanner;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

//public class Main {
public class FourWins extends JApplet implements MouseListener  {

	private static int max_depth = 6; 	// maximal recursion depth
										// for minimax algorithm
	private static int minimax_wins = 0;
	private static int random_wins = 0;
	int m_board[] = new int[42];
	
	Graphics m_graphics;
	int m_xpos;
	int m_ypos;
	int m_state = 0;				// 0=player input 1=CPU thinking
									// 2 =game over
	int m_board_width = 400;
	int m_board_height = 330;
	int m_left = 10;
	int m_top = 10;
	
	public static void main(String args[])
	{
		System.out.println("test");
	}
	
	/**
	 * @return
	 * return heuristic value of the situation for
	 * player 0 (human), where 0 means player 0 (X, human), 1
	 * means player 1 (0, CPU) and 2 means empty field 
	 * @param 
	 * board: 	array of field values (containing
	 * 			42 elements)
	 */
	public static int heuristic(int board[])
	{
		int x1=0, x2=0, x3=0;					// number of 1/2/3 in a 4-set
		boolean x4=false;						// is there a full 4-set for X?
		int o1=0, o2=0, o3=0;
		boolean o4=false;
		
		// count along rows
		for(int i = 0; i < 6; i++)	// row
		{
			for(int j = 0; j < 7 - 3; j++) // start col
			{
				int nx = 0, no = 0;
				for(int k = 0; k < 4; k++)	// 4-set
				{
					int idx = i*7 + (j+k);
					int val = board[idx];
					switch(val)
					{
					case 0:
						nx++;
						break;
					case 1:
						no++;
						break;
					default:
						break;
					}
				}
				
				if(no == 0)
				{
					switch(nx)
					{
					case 4:
						x4 = true;
						break;
					case 3:
						x3++;
						break;
					case 2:
						x2++;
						break;
					case 1:
						x1++;
						//System.out.println("found horizontal 1 out of 4");
						break;
					default:
						break;
					}
				}
				
				if(nx == 0)
				{
					switch(no)
					{
					case 4:
						o4 = true;
						break;
					case 3:
						o3++;
						break;
					case 2:
						o2++;
						break;
					case 1:
						o1++;
						break;
					default:
						break;
					}
				}
			}
		}
		
		// count along cols
		for(int i = 0; i < 7; i++) // col
		{
			for(int j = 0; j < 6-3; j++) // start row
			{
				int nx = 0, no = 0;
				for(int k = 0; k < 4; k++)	// 4-set
				{
					int idx = (j+k)*7+i;
					int val = board[idx];

					switch(val)
					{
					case 0:
						nx++;
						break;
					case 1:
						no++;
						break;
					default:
						break;
					}
				}
				
				if(no == 0)
				{
					switch(nx)
					{
					case 4:
						x4 = true;
						break;
					case 3:
						x3++;
						break;
					case 2:
						x2++;
						break;
					case 1:
						x1++;
						//System.out.println("found vertical 1 out of 4");
						break;
					default:
						break;
					}
				}
				
				if(nx == 0)
				{
					switch(no)
					{
					case 4:
						o4 = true;
						break;
					case 3:
						o3++;
						break;
					case 2:
						o2++;
						break;
					case 1:
						o1++;
						break;
					default:
						break;
					}
				}
			}
		}
		
		// count along diags from top left to bottom right
		for(int i = 0; i < 6-3; i++)	// row
		{
			for(int j = 0; j < 7-3; j++) // col
			{
				int nx = 0, no = 0;
				for(int k = 0; k < 4; k++)	// 4-set
				{
					int idx = (i+k)*7+(j+k);
					int val = board[idx];
					
					switch(val)
					{
					case 0:
						nx++;
						break;
					case 1:
						no++;
						break;
					default:
						break;
					}
				}
				
				if(no == 0)
				{
					switch(nx)
					{
					case 4:
						x4 = true;
						break;
					case 3:
						x3++;
						break;
					case 2:
						x2++;
						break;
					case 1:
						x1++;
						//System.out.println("found falling diag 1 out of 4" );
						break;
					default:
						break;
					}
				}
				
				if(nx == 0)
				{
					switch(no)
					{
					case 4:
						o4 = true;
						break;
					case 3:
						o3++;
						break;
					case 2:
						o2++;
						break;
					case 1:
						o1++;
						break;
					default:
						break;
					}
				}
			}
		}
		
		// count along diags from top right to the bottom left
		for(int i = 6; i > 2; i--)			// col
		{
			for(int j = 0; j < 6-3; j++)	// row
			{
				int nx = 0, no = 0;
				for(int k = 0; k < 4; k++)	// 4-set	
				{
					int idx = (j+k)*7 + (i-k);
					int val = board[idx];
					
					switch(val)
					{
					case 0:
						nx++;
						break;
					case 1:
						no++;
						break;
					default:
						break;
					}
				}
				
				if(no == 0)
				{
					switch(nx)
					{
					case 4:
						x4 = true;
						break;
					case 3:
						x3++;
						break;
					case 2:
						x2++;
						break;
					case 1:
						x1++;
						//System.out.println("found rising diag 1 out of 4");
						break;
					default:
						break;
					}
				}
				
				if(nx == 0)
				{
					switch(no)
					{
					case 4:
						o4 = true;
						break;
					case 3:
						o3++;
						break;
					case 2:
						o2++;
						break;
					case 1:
						o1++;
						break;
					default:
						break;
					}
				}
			}
		}
		
		// compute heuristic value
		int value = x1 + x2*10 + x3*100 - o1 - o2*1 - o3*100;
		if(x4 == true)
			value += 1000;
		if(o4 == true)
		{
			value -= 1000;
		}
		
		/*
		System.out.println("***********");
		System.out.println("heuristics:");
		System.out.println("x1=" + x1 + " x2=" + x2 + " x3=" + x3 + " x4=" + x4);
		System.out.println("o1=" + o1 + " o2=" + o2 + " o3=" + o3 + " o4=" + o4);
		System.out.println("***********");
		*/
		
		return value;
	}

	/**
	 * @return
	 * recursively walk the tree and return the
	 * value of the current situation in ret[],
	 * where ret[0] is the minimax-rating of board[]
	 * and ret[1] is the field associated to this rating
	 * ret[1] = -1 indicates a leaf
	 * @param 
	 * board: 	array of field values (containing
	 * 			42 elements)
	 * p:		current player, either 0 (human, X) 
	 * 			or 1 (cpu, O)
	 * depth: 	recursion depth of this level
	 */
	public static void minimax(int board[], int p, int depth, int ret[])
	{		
		// if maximal recursion depth is reached,
		// just return the evaluation of the leaf
		if(depth == max_depth)
		{
			ret[0] = heuristic(board);
			ret[1] = -1;
			//System.out.println("max depth reached at depth " + depth);
			return;
		}
		
		// if board is full,
		// just return the evaluation of the leaf
		boolean full = true;
		for(int i = 0; i < 42; i++)
		{
			if(board[i] == 2)
			{
				full = false;
				break;
			}
		}
		if(full)
		{
			ret[0] = heuristic(board);
			ret[1] = -1;
			//System.out.println("full board reached with rating " + ret[0] + 
			//		" at level " + depth + ":");
			//printBoard(board);
			return;
		}
		
		// if board has a winner,
		// just return the evaluation of the leaf
		if(winner(board) != 2)
		{
			ret[0] = heuristic(board);
			ret[1] = -1;
			//System.out.println("winning board reached with rating " + ret[0] + 
			//		" at level " + depth + ":");
			//printBoard(board);
			return;
		}
		
		// evaluate all the possible successive moves
		int maxmove[] = {0, -1};			// maximizing move
		int minmove[] = {0, -1};				// minimizing move
		
		// rate all possible moves
		for(int i = 0; i < 7; i++)
		{	
			/*if(board[i] != 2)
			{
				continue;
			}*/
			if(colfull(board,i))
			{
				continue;
			}

			//System.out.println("trying field " + i + " at level " + depth);
			
			// perform move and minimax-rating (recursively)
			int newboard[] = board.clone();
			//newboard[i] = p;
			put(p,i,newboard);
			//printBoard(newboard);
			int newp;
			if(p == 0)
				newp = 1;
			else
				newp = 0;
			int res[] = {0,0};
			minimax(newboard, newp, depth + 1, res);
			//System.out.println("field " + i + " has rating " + res[0] + 
			//		" at level " + depth);
			
			// update minimal and maximal ratings
			// where appropriate
			if(res[0] > maxmove[0] || maxmove[1] < 0)
			{
				maxmove[0] = res[0];
				maxmove[1] = i;
			}
			if(res[0] < minmove[0] || minmove[1] < 0)
			{
				minmove[0] = res[0];
				minmove[1] = i;
			}
		}
		
		// choose the maximal/minimal move
		if(p == 0)		// MAX
		{
			ret[0] = maxmove[0];
			ret[1] = maxmove[1];
		}
		if(p == 1)		// MIN
		{
			ret[0] = minmove[0];
			ret[1] = minmove[1];
		}
		
		//System.out.println("choose field " + ret[1] + " with rating " + ret[0] + " at level " + depth);
	}
	
	/**
	 * @return
	 * Return, where the CPU wants to put next X.
	 * This is a dummy AI, which just puts the X
	 * randomly.
	 * @param 
	 * test: 	array of field values (containing
	 * 			9 elements)
	 */
	public static int ai_random(int board[])
	{	
		int pos = -1;
		int nchecks = 0;
		Random rnd = new Random();
		while(pos < 0 && nchecks < 1000)
		{
			int check = rnd.nextInt(7);
			if(colfull(board, check) == false)
			{
				pos = check;
			}
		}
		
		return pos;
	}
	
	/**
	 * @return
	 * Return, where the CPU wants to put next X.
	 * This AI uses the minimax algorithm
	 * @param 
	 * test: 	array of field values (containing
	 * 			42 elements)
	 */
	public static int ai_minimax(int board[])
	{
		int ret[] = {0,0};
		minimax(board, 1, 0, ret);
				
		return ret[1];
	}	
		
	/**
	 * @return
	 * return 0 if human (X) wins, 1 if CPU (O) wins,
	 * return 2 if nobody wins
	 * @param 
	 * test: 	array of field values (containing
	 * 			42 elements)
	 */
	public static int winner(int board[])
	{
		// check rows
		for(int i = 0; i < 6; i++)	// row
		{
			for(int j = 0; j < 7-3; j++)	// col
			{
				int nx = 0, no=0;
				for(int k = 0; k < 4; k++)
				{
					int idx = i*7 + (j+k);
					switch(board[idx])
					{
					case 0:
						nx++;
						break;
					case 1:
						no++;
						break;
					}
				}
				
				if(nx == 4)
				{
					return 0;
				}
				if(no == 4)
				{
					return 1;
				}
			}
		}
		
		// check cols
		for(int i = 0; i < 6-3; i++)	// row
		{
			for(int j = 0; j < 7; j++)	// col
			{
				int nx = 0, no=0;
				for(int k = 0; k < 4; k++)
				{
					int idx = (i+k)*7 + j;
					switch(board[idx])
					{
					case 0:
						nx++;
						break;
					case 1:
						no++;
						break;
					}
				}
				
				if(nx == 4)
				{
					return 0;
				}
				if(no == 4)
				{
					return 1;
				}
			}
		}
		
		// check raising diags
		for(int i = 0; i < 6-3; i++)	// row
		{
			for(int j = 3; j < 7; j++)	// col
			{
				int nx = 0, no=0;
				for(int k = 0; k < 4; k++)
				{
					int idx = (i+k)*7 + (j-k);
					switch(board[idx])
					{
					case 0:
						nx++;
						break;
					case 1:
						no++;
						break;
					}
				}
				
				if(nx == 4)
				{
					return 0;
				}
				if(no == 4)
				{
					return 1;
				}
			}
		}
		
		// check falling diags
		for(int i = 0; i < 6-3; i++)	// row
		{
			for(int j = 0; j < 7-3; j++)	// col
			{
				int nx = 0, no=0;
				for(int k = 0; k < 4; k++)
				{
					int idx = (i+k)*7 + (j+k);
					switch(board[idx])
					{
					case 0:
						nx++;
						break;
					case 1:
						no++;
						break;
					}
				}
				
				if(nx == 4)
				{
					return 0;
				}
				if(no == 4)
				{
					return 1;
				}
			}
		}
		
		return 2;
	}		

	/**
	 * @return
	 * returns if the board is full
	 */
	public static boolean full(int board[])
	{
		for(int i = 0; i < 42; i++)
		{
			if(board[i] == 2)
				return false;
		}
		
		return true;
	}
	
	/**
	 * @return
	 * returns if column x is full
	 */	
	public static boolean colfull(int board[], int x)
	{
		for(int i = 0; i < 6; i++)
		{
			int idx = i*7+x;
			if(board[idx] == 2)
				return false;
		}
		
		return true;
	}

	/**
	 * @return
	 * insert the stone of player p at x
	 * and return the resulting board
	 * return false if out of bounds or column full
	 */
	public static boolean put(int p, int x, int board[])
	{		
		if(x < 0 || x > 6)
			return false;
		
		if(colfull(board, x))
			return false;
		
		int row = 0;
		for(int i = 1; i < 6; i++)
		{
			int idx = i*7+x;
			if(board[idx] != 2)
			{
				break;
			}
			row++;
		}
		
		board[row*7+x] = p;
		return true;
	}
	
	/**
	 * @return
	 * generates a random board and stores it to board
	 */
	public static void randomize(int nstones, int board[])
	{
		Random rnd = new Random();
		int p = 0;
		
		for(int i = 0; i < nstones; i++)
		{	
			
			if(full(board))
				break;
			
			boolean fullcol;
			int x;
			do
			{
				x = rnd.nextInt(7);
				fullcol = colfull(board, x);
			}while(fullcol == true);
			
			put(p, x, board);
			
			if(p == 0)
				p = 1;
			else
				p = 0;
		}
	}	
	
	/**
	 * @return
	 * return empty board
	 */
	public static void emptyBoard(int board[])
	{
		for(int i = 0; i < 42; i++)
		{
			board[i] = 2;
		}
	}

	/**
	 * @return
	 * return inverted board
	 */
	public static void invertBoard(int board[])
	{
		for(int i = 0; i < 42; i++)
		{
			if(board[i] == 0)
				board[i] = 1;
			else if(board[i] == 1)
				board[i] = 0;
		}
	}
	
	/**
	 * @return
	 * draws the board, where 0 means player 0 (X, Human), 1
	 * means player 1 (0, CPU) and 2 means empty field 
	 * @param 
	 * test: 	array of field values (containing
	 * 			42 elements)
	 */
	public static void printBoard(int test[])
	{
		System.out.print("---------------\n");
		for(int i = 0; i < 6; i++)
		{
			System.out.print("|");
			for(int j = 0; j < 7; j++)
			{
				int val = test[i*7+j];
				switch(val)
				{
				case 0:
					System.out.print("X");	
					break;
				case 1:
					System.out.print("O");
					break;
				default:
					System.out.print(" ");
					break;	
				}
				System.out.print("|");
			}
			System.out.print("\n");
		}
		System.out.print("---------------\n");
		System.out.print("|0 1 2 3 4 5 6|\n");		
		System.out.print("---------------\n");
	}	

	/**
	 * @param 
	 * board: 	array of field values (containing
	 * 			42 elements) to be drawn
	 */
	public void drawBoard(int board[])
	{

		// draw the empty board
		m_graphics.drawRect(m_left, m_top, m_board_width, m_board_height);
		for(int i = 1; i < 7; i++)
		{
			int step = (int)(m_board_width/7.0);
			int x1 = m_left + i*step;
			int y1 = m_top;
			int x2 = m_left + i*step;
			int y2 = y1 + m_board_height;
			m_graphics.drawLine(x1, y1, x2, y2);
		}
		m_graphics.setColor(Color.lightGray);
		for(int i = 1; i < 6; i++)
		{
			int step = (int)(m_board_height/6.0);
			int x1 = m_left;
			int y1 = m_top + i*step;
			int x2 = m_left + m_board_width;
			int y2 = m_top + i*step;
			m_graphics.drawLine(x1, y1, x2, y2);
		}
		
		
		// draw the chips
		int xstep = (int)(m_board_width/7.0);
		int ystep = (int)(m_board_height/6.0);
		for(int i = 0; i < 6; i++)		// row
		{
			for(int j = 0; j < 7; j++)	// col
			{
				int idx = i*7+j;
				int val = board[idx];
				
				if(val == 2)
					continue;
				
				int x = j*xstep + m_left;
				int y = i*ystep + m_top;		 
				m_graphics.setColor(Color.black);
				m_graphics.drawOval(x, y, xstep, ystep);
				if(val == 0)
					m_graphics.setColor(Color.blue);
				if(val == 1)
					m_graphics.setColor(Color.red);
				m_graphics.fillOval(x, y, xstep, ystep);
				
			}
		}
	}
	
	/**
	 * do the connect four game
	 */
	public static void fourwins()
	{
		int board[] = new int[42];
		emptyBoard(board);
		Scanner scanner = new Scanner(System.in);
	    
	    do
	    {
	    	printBoard(board);
	    	boolean good_input;
	    	do
	    	{
	    		System.out.print("input: ");
	    		int pos = scanner.nextInt();
	    		good_input = put(0, pos, board);	
	    	}while(good_input == false);
	    }while(full(board) == false);
	}
	
	/**
	 * @return
	 * prompt for player input until a valid field
	 * has been chosen
	 * @param 
	 * test: 	array of field values (containing
	 * 			42 elements)
	 */
	public static int human_prompt_text(int board[])
	{	
		while(true)
		{
			Scanner scanner = new Scanner(System.in);
			System.out.print("input: ");
		    int pos = scanner.nextInt();
		    
		    if(pos >= 0 && pos < 9)
		    {
		    	int val = board[pos];
		    	if(val==2)
		    	{
		    		return pos;
		    	}
		    }
		}
	}
	
	/**
	 * @return
	 * prompt for player input until a valid field
	 * has been chosen
	 * @param 
	 * test: 	array of field values (containing
	 * 			42 elements)
	 */
	public static int human_prompt(int board[])
	{	
		while(true)
		{
			Scanner scanner = new Scanner(System.in);
			System.out.print("input: ");
		    int pos = scanner.nextInt();
		    
		    if(pos >= 0 && pos < 9)
		    {
		    	int val = board[pos];
		    	if(val==2)
		    	{
		    		return pos;
		    	}
		    }
		}
	}
	
	/**
	 * @return
	 * perform a game between cpu and human
	 * @param 
	 * human_first: true if human starts, false
	 * 				if CPU starts
	 */
	public static void connectfour(boolean human_first)
	{
		int board[] = new int[42];
		emptyBoard(board);
		int curr_move;
		int who_wins = 2;
		if(human_first)
			curr_move = 0;
		else
			curr_move = 1;
	
		do
		{
			printBoard(board);
			if(full(board))
				break;
			if(curr_move == 0)	// human's turn
			{
				int pos = human_prompt_text(board);
				//int pos = ai_random(board);
				//Scanner scanner = new Scanner(System.in);
			    //String dummy = scanner.nextLine();
				put(0, pos, board);
				who_wins = winner(board);
				curr_move = 1;
			}
			else				// CPU's turn
			{
				//int pos = ai_random(board);
				int pos = ai_minimax(board);
				put(1, pos, board);
				who_wins = winner(board);
				curr_move = 0;
			}
		}while(who_wins == 2);
		
		printBoard(board);
		switch(who_wins)
		{
		case 0:
			System.out.println("human wins");
			random_wins++;
			break;
		case 1:
			System.out.println("CPU wins");
			minimax_wins++;
			break;
		default:
			System.out.println("game draw");
			break;
		}
	}
	
	/**
	 * @param args
	 */
	//public static void main(String[] args) {

		//int board[] = new int[42];
		/*emptyBoard(board);
		randomize(4, board);*/
		/*board[5*7 + 3] = 0;
		board[4*7 + 3] = 0;
		board[4*7 + 4] = 0;
		board[3*7 + 4] = 1;
		board[5*7 + 4] = 1;
		board[2*7 + 4] = 0;*/
		/*board[5*7 + 3] = 1;
		board[4*7 + 3] = 1;
		board[4*7 + 4] = 1;
		board[3*7 + 4] = 0;
		board[5*7 + 4] = 0;
		board[2*7 + 4] = 1;*/
		/*board[5*7 + 0] = 0;
		board[5*7 + 1] = 0;
		board[5*7 + 2] = 0;
		board[5*7 + 3] = 0;*/
		/*printBoard(board);
		heuristic(board);*/
		
		/*
		Scanner s = new Scanner(System.in);
		for(int i = 0; i < 100; i++)
		{
			emptyBoard(board);
			randomize(20, board);
			int w = winner(board);
			if(w == 0)
			{
				printBoard(board);
				System.out.println("X won");
				s.nextLine();
			}
			else if(w == 1)
			{
				printBoard(board);
				System.out.println("O won");
				s.nextLine();
			}
			else
			{
				System.out.println("No winner");
			}
				
		}
		*/
		
		/*
		for(int i = 0; i < 100; i++)
		{
			connectfour(true);
		}
		
		System.out.println("Random wins: " + random_wins);
		System.out.println("Minimax wins: " + minimax_wins);		
			
		//fourwins();
		
	}
	*/
		
	public void mouseEntered (MouseEvent me) {}
	public void mousePressed (MouseEvent me) {}
	public void mouseReleased (MouseEvent me) {} 
	public void mouseExited (MouseEvent me) {}  
	
	public void mouseClicked (MouseEvent me) 
	{		
		// discard input if CPU thinking or game over
		if(m_state == 1)
		{
			System.out.println("discarded");
			return;
		}
		if(m_state == 2)
		{
			System.out.println("game over");
			return;
		}
		
		// switch to human input mode
		m_state = 0;
		
		// determine column which has been clicked
		m_xpos = me.getX();
		m_ypos = me.getY();
		int xstep = (int)(m_board_width/7.0);
		int col = -1;
		
		for(int i = 0; i < 7; i++) 
		{
			int x1 = i*xstep + m_left;
			int x2 = x1 + xstep;
			int y1 = m_top;
			int y2 = m_top + m_board_height; 
			
			if(m_xpos > x1 && m_xpos < x2 && m_ypos > y1 && m_ypos < y2)
			{
				col = i;
				break;
			}
		}
		if(col == -1)
			return;
		if(colfull(m_board, col))
		{
			return;
		}
		
		// put at this position
		System.out.println("insert at col: " + col);
		put(0, col, m_board);
		drawBoard(m_board);
		repaint();
		int who_wins = winner(m_board);
		if(who_wins == 0)
		{
			System.out.println("human wins");
			m_state = 2;
			return;
		}
		if(who_wins == 1)
		{
			System.out.println("CPU wins");	
			m_state = 2;
			return;
		}
		
		// perform turn of CPU
		m_state = 1;				// lock user input
		//int pos = ai_random(m_board);
		System.out.println("thinking..");
		int pos = ai_minimax(m_board);
		put(1, pos, m_board);
		drawBoard(m_board);
		repaint();
		who_wins = winner(m_board);
		if(who_wins == 0)
		{
			System.out.println("human wins");
			JOptionPane.showMessageDialog(null, "You win!");
			m_state = 2;
			return;
		}
		if(who_wins == 1)
		{
			System.out.println("CPU wins");	
			JOptionPane.showMessageDialog(null, "CPU wins");
			m_state = 2;
			return;
		}
		
		m_state = 0;				// unlock user input 

	} 
	
	public void start()
	{
		emptyBoard(m_board);
		//randomize(20, m_board);
		printBoard(m_board);
		addMouseListener(this); 
		JOptionPane.showMessageDialog(null, "Controls: \nClick on the desired row to place a stone");
		//connectfour(true);
	}
	
	public void paint(Graphics g) 
	{
		m_graphics = g; 
		printBoard(m_board);
		drawBoard(m_board);
	}
}
