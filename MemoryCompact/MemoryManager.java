

import java.util.TreeMap;

public class MemoryManager
	{
	TreeMap<Integer, Area> free;
	TreeMap<Integer, Area> used;

	public MemoryManager(int size)
		{

			free = new TreeMap<Integer, Area>();
			used = new TreeMap<Integer, Area>();
			free.put(0, new Area(0, size));
		}

	public Area aquire(int amount)
		{
            Area area = null; //declare variable to be returned, null if no space is available
			for (Integer i:free.keySet()) {
				if (free.get(i).getSize() >= amount) { //ensure there is a big enough memory space
					int start = free.get(i).getStart(); //get the start of the space to be pulled from memory
					used.put(start, new Area(start, start + amount)); //declare new used spot
					free.put(start + amount, new Area(start + amount, free.get(i).getEnd())); // add new area after the acquired memory
					free.remove(i); //remove
                    area = new Area(start, start + amount); //area to be returned
                    break;
				}
			}
            return area; //only want one return point
		}

	public void release(int handle)
		{
			int start =handle;  //declare start for the new "free" block
			for (Integer i: free.keySet()) {
				if (free.get(i).getEnd() == handle) { //check if there is a predecessor to the block
					start = i;  //update start if a predecessor exists
					free.remove(i); //remove the predecessor
					break;
				}
			}
			int handleEnd = used.get(handle).getEnd();
			int end = free.containsKey(handleEnd)?free.get(handleEnd).getEnd():handleEnd; //get the end of memory area, whether it is handles block or a block afterwards
			if (handleEnd != end) {
				free.remove(handleEnd); //remove block immediately following
			}
			free.put(start, new Area(start, end)); //add new freed block
			used.remove(handle); //remove block from used


		}

	public void print()
		{
			synchronized (free)
				{
					System.out.println("\nFree");
					free.forEach((k, v) -> System.out.println(k + "->" + v.getEnd() + "(" + v.getSize() + ")"));
					System.out.println("Used");
					used.forEach((k, v) -> System.out.println(k + "->" + v.getEnd() + "(" + v.getSize() + ")"));
				}
		}

	public Area getFirstFree()
		{
			return free.firstEntry().getValue();
		}

	}
