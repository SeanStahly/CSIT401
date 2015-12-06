import org.junit.Test;


public class Test1
	{

	@Test
	public void test()
		{
			MemoryManager m = new MemoryManager(10);
			m.aquire(1);
			org.junit.Assert.assertTrue(m.getFirstFree().getStart() == 1);
			m.aquire(1);
			org.junit.Assert.assertTrue(m.getFirstFree().getStart() == 2);
			m.aquire(1);
			org.junit.Assert.assertTrue(m.getFirstFree().getStart() == 3);
			m.release(0);
			m.release(2);
			m.print();
			m.release(1);
			org.junit.Assert.assertTrue(m.getFirstFree().getStart() == 0);
			org.junit.Assert.assertTrue(m.getFirstFree().getEnd() == 10);
		}

	@Test
	public void test2()
		{
			MemoryManager m = new MemoryManager(10);
			Area Area1 = m.aquire(1);
			Area Area2 = m.aquire(1);
			org.junit.Assert.assertTrue(m.getFirstFree().getStart() == 2);
			org.junit.Assert.assertTrue(m.getFirstFree().getEnd() == 10);
			m.print();
			m.release(Area1.getStart());
			org.junit.Assert.assertTrue(m.getFirstFree().getStart() == 0);
			org.junit.Assert.assertTrue(m.getFirstFree().getEnd() == 1);
			m.print();
			m.release(Area2.getStart());
			m.print();
			org.junit.Assert.assertTrue(m.getFirstFree().getStart() == 0);
			org.junit.Assert.assertTrue(m.getFirstFree().getEnd() == 10);
		}
	}
