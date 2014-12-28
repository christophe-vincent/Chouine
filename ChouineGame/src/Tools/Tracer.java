/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package Tools;

/**
 *
 * @author vincentc
 */
public class Tracer
{
    public enum Verbosity
    {
        NONE(0), ERROR(1), WARNING(2), INFO(3), DEBUG(4);
        private final int value;

        private Verbosity(int value)
        {
            this.value = value;
        }

        public int getValue()
        {
            return value;
        }
    };
    static Verbosity m_Level;
    
    public Tracer()
    {
        m_Level = Verbosity.NONE;
    }
    
    public Tracer(Verbosity _level)
    {
        m_Level = _level;
    }
    
    static public void SetVerbosity(Verbosity _level)
    {
        m_Level = _level;
    }
    
    static public void println(Verbosity _level, String _text)
    {
        if ((_level.getValue() <= m_Level.getValue()) || (_level == Verbosity.DEBUG))
        {
            System.out.println(_text);
        }
    }
    
    static public void print(Verbosity _level, String _text)
    {
        if ((_level.getValue() <= m_Level.getValue()) || (_level == Verbosity.DEBUG))
        {
            System.out.print(_text);
        }
    }
}
