

public class Process implements Runnable
	{
	MemoryManager memory;


	public Process(MemoryManager memory)
		{
			super();
			this.memory = memory;
		}

	public void run()
		{
			memory.print();
			int amount = (int) (Math.random() * 1000);
			Area handle = memory.aquire(amount);
			memory.print();
			try
				{
					Thread.sleep(1000 + (int) Math.random() * 100);
				}
			catch (InterruptedException e)
				{
					e.printStackTrace();
				}
			if (handle != null)
				memory.release(handle.getStart());
			memory.print();
		}


	}
