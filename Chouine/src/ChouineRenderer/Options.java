/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package ChouineRenderer;

/**
 *
 * @author VINCENTC
 */
public class Options
{

    static private float m_RemoveCardsTimeOut = 2.0f;
    static private boolean m_ShowCardToPlay = false;
    static private int m_ComputerLevel = 10;

    static public void setRemoveCardsTimeOut(float _timeout)
    {
        m_RemoveCardsTimeOut = _timeout;
    }

    static public float getRemoveCardsTimeOut()
    {
        return m_RemoveCardsTimeOut;
    }

    static void setShowCardToPlay(boolean _show)
    {
        m_ShowCardToPlay = _show;
    }

    static boolean getShowCardToPlay()
    {
        return m_ShowCardToPlay;
    }

    static void setComputerLevel(String _level)
    {
        if ("cl_low".equals(_level))
        {
            m_ComputerLevel = 0;
        }
        if ("cl_medium".equals(_level))
        {
            m_ComputerLevel = 1;
        }
        if ("cl_high".equals(_level))
        {
            m_ComputerLevel = 6;
        }
    }
    
    static int getComputerLevel()
    {
        return m_ComputerLevel;
    }
}
