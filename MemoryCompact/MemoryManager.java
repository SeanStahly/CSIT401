

import java.util.TreeMap;
import java.util.concurrent.ConcurrentSkipListMap;

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
        int start = -1;
        synchronized (free){
            for (Integer i:free.keySet()) {
                if (free.get(i) != null && free.get(i).getSize() >= amount) {
                    if (free.get(i) != null && free.get(i).getSize() >= amount) { //ensure there is a big enough memory space
                        start = free.get(i).getStart(); //get the start of the space to be pulled from memory
                        area = new Area(start, start + amount); //area to be returned
                        free.put(start + amount, new Area(start + amount, free.get(i).getEnd())); // add new area after the acquired memory
                        free.remove(i); //remove starting block

                        break;
                    }
                }

            }
        }
        if (area != null && start >= 0) {
            synchronized (used) {
                used.put(start, area); //declare new used spot
            }
        }
        return area; //only want one return point
    }

    public void release(int handle)
    {
        int start =handle;  //declare start for the new "free" block
        int handleEnd = -1;
        synchronized (used) {
            handleEnd = used.get(handle).getEnd();
        }
        if (handleEnd >= 0) {
            synchronized (free) {
                for (Integer i : free.keySet()) {
                    if (free.get(i) != null && free.get(i).getEnd() == handle) { //check if there is a predecessor to the block
                        start = i;  //update start if a predecessor exists
                        free.remove(i); //remove the predecessor
                        break;
                    }
                }
                int end = free.containsKey(handleEnd) ? free.get(handleEnd).getEnd() : handleEnd; //get the end of memory area, whether it is handles block or a block afterwards
                if (handleEnd != end) {
                    free.remove(handleEnd); //remove block immediately following
                }
                free.put(start, new Area(start, end)); //add new freed block
            }
            synchronized (used) {
                used.remove(handle); //remove block from used
            }

        }

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
