

public class Area
	{
	int start;
	int end;

	public Area(int start, int end)
		{
			super();
			this.start = start;
			this.end = end;
		}


	public int getStart()
		{
			return start;
		}

	public void setStart(int start)
		{
			this.start = start;
		}

	public int getEnd()
		{
			return end;
		}

	public void setEnd(int end)
		{
			this.end = end;
		}

	public int getSize()
		{
			return end - start;
		}

	public void setSize(int x)
		{
			end = start + x;
		}
	}
