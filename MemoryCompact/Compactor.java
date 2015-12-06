

import java.util.Scanner;

public class Compactor
	{
	public static Boolean done = false;

	public static void main(String[] args) throws InterruptedException
		{
			System.out.println("Enter 1 for case 1 and 2 for loop");
			Scanner s = new Scanner(System.in);
			int caseX = s.nextInt();
			switch (caseX)
				{
				case 1:
					MemoryManager m = new MemoryManager(10000);
					Process p = new Process(m);
					new Thread(p).start();
					Process p2 = new Process(m);
					new Thread(p2).start();
					Process p3 = new Process(m);
					new Thread(p3).start();
					break;
				case 2:
					MemoryManager n = new MemoryManager(10000);
					ReadInput r = new ReadInput(s);
					r.start();
					while (!done)
						{
							Process x = new Process(n);
							new Thread(x).start();
							Thread.sleep(100);
						}
					break;
				}

		}

	}
