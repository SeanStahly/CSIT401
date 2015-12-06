

import java.util.Scanner;

public class ReadInput extends Thread
	{
	Scanner s;

	public ReadInput(Scanner t)
		{
			s = t;

		}

	public void run()
		{
			s.nextInt();
			Compactor.done = true;
		}
	}
