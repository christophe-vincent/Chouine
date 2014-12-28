/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package Cards;

import java.util.LinkedList;
import java.util.List;

/**
 *
 * @author vincentc
 */
public class Card
{
    public enum Color
    {
        COEUR, PIC, TREFLE, CARREAU, BACK;
    };
    
    public static final Color[] ALL_COLORS =
    {
        Color.CARREAU,
        Color.COEUR,
        Color.PIC,
        Color.TREFLE
    };

    public enum CardValue
    {
        SEPT(0), HUIT(1), NEUF(2), VALET(3), DAME(4), ROI(5), DIX(6), AS(7);
        private final int value;

        private CardValue(int value)
        {
            this.value = value;
        }

        public int getValue()
        {
            return value;
        }
    };
    public static final CardValue[] ALL_VALUES =
    {
        CardValue.SEPT,
        CardValue.HUIT,
        CardValue.NEUF,
        CardValue.VALET,
        CardValue.DAME,
        Card.CardValue.ROI,
        CardValue.DIX,
        Card.CardValue.AS 
     /*   CardValue.VALET,
        CardValue.HUIT,
        CardValue.DAME,
        CardValue.NEUF,
        Card.CardValue.ROI,
        CardValue.SEPT,
        CardValue.DIX,
        Card.CardValue.AS */
        
  /*      CardValue.VALET,
        CardValue.VALET,
        CardValue.DAME,
        CardValue.HUIT,
        Card.CardValue.ROI,
        CardValue.NEUF,
        CardValue.DIX,
        CardValue.SEPT,
        Card.CardValue.AS*/
    };
    
    
    private Color m_Color;
    private CardValue m_Value;
    private boolean m_Trump;
    private Announce m_Announce;
    private List<Announce> m_ProbableAnnounce;
    private List<Announce.AnnounceType> m_Announcetypes;
    private int m_Score;

    public Card(Color _color, CardValue _value, boolean _trump)
    {
        m_Color = _color;
        m_Value = _value;
        m_Trump = _trump;
        m_Announce = null;
        m_ProbableAnnounce = new LinkedList();
        m_Announcetypes = new LinkedList();
        m_Score = 0;
        
        switch (m_Value)
        {
            case AS: 
                m_Announcetypes.add(Announce.AnnounceType.CHOUINE);
                m_Announcetypes.add(Announce.AnnounceType.QUARANTE);
                m_Announcetypes.add(Announce.AnnounceType.QUINTE);
                break;
            case DIX:
                m_Announcetypes.add(Announce.AnnounceType.CHOUINE);
                m_Announcetypes.add(Announce.AnnounceType.QUINTE);
                break;
            case ROI:
                m_Announcetypes.add(Announce.AnnounceType.CHOUINE);
                m_Announcetypes.add(Announce.AnnounceType.QUARANTE);
                m_Announcetypes.add(Announce.AnnounceType.TIERCE);
                m_Announcetypes.add(Announce.AnnounceType.MARIAGE);
                break;
            case DAME:
                m_Announcetypes.add(Announce.AnnounceType.CHOUINE);
                m_Announcetypes.add(Announce.AnnounceType.QUARANTE);
                m_Announcetypes.add(Announce.AnnounceType.TIERCE);
                m_Announcetypes.add(Announce.AnnounceType.MARIAGE);
                break;
            case VALET:
                m_Announcetypes.add(Announce.AnnounceType.CHOUINE);
                m_Announcetypes.add(Announce.AnnounceType.QUARANTE);
                m_Announcetypes.add(Announce.AnnounceType.TIERCE);
                break;
        }
    }

    public Color color()
    {
        return m_Color;
    }

    public int value()
    {
        return m_Value.getValue();
    }

    public CardValue cardValue()
    {
        return m_Value;
    }

    public int getPoints()
    {
        int points;

        switch (m_Value)
        {
            case SEPT:
            case HUIT:
            case NEUF:
                points = 0;
                break;
            case VALET:
                points = 2;
                break;
            case DAME:
                points = 3;
                break;
            case ROI:
                points = 4;
                break;
            case DIX:
                points = 10;
                break;
            case AS:
                points = 11;
                break;
            default:
                points = 0;
        }
        return points;
    }
    
    public void setAnnounce(Announce _ann)
    {
        m_Announce = _ann;
    }

    public Announce getAnnounce()
    {
        return m_Announce;
    }
    
    public List<Announce> getProbableAnnounces()
    {
        return m_ProbableAnnounce;
    }
    
    public void addProbableAnnounce(Announce _announce)
    {
        if (!m_ProbableAnnounce.contains(_announce))
        {
            m_ProbableAnnounce.add(_announce);
        }
    }
    
    public void removeProbableAnnounce(Announce _announce)
    {
        if (m_ProbableAnnounce.contains(_announce))
        {
            m_ProbableAnnounce.remove(_announce);
        }
    }
    
    public List<Announce.AnnounceType> getAllAnnounceTypes()
    {
        return m_Announcetypes;
    }
    
    public int getScore()
    {
        return m_Score;
    }
    
    public void setScore(int _score)
    {
        m_Score = _score;
    }
            
    
    public boolean isBetter(Card _card)
    {
        boolean ret;

        if (m_Color == _card.color())
        {
            // same color for both
            if (value() > _card.value())
            {
                ret = false;
            } else
            {
                ret = true;
            }
        } else
        {
            if (_card.isTrump())
            {
                ret = true;
            } else
            {
                ret = false;
            }
        }
        return ret;
    }

    public static List<Card> getColorCards(Card.Color _color, List<Card> _list)
    {
        List<Card> select = new LinkedList();

        for (Card c : _list)
        {
            if (c.color() == _color)
            {
                select.add(c);
            }
        }

        return select;
    }

    public static boolean containsValue(CardValue _value, List<Card> _list)
    {
        boolean ret = false;

        for (Card c : _list)
        {
            if (c.cardValue() == _value)
            {
                ret = true;
            }
        }

        return ret;
    }
    
    public static Card getCardFromList(CardValue _value, List<Card> _list)
    {
        Card ret = null;

        for (Card c : _list)
        {
            if (c.cardValue() == _value)
            {
                ret = c;
            }
        }

        return ret;
    }

    public static int NumberOf(CardValue _value, List<Card> _list)
    {
        int ret = 0;

        for (Card c : _list)
        {
            if (c.cardValue() == _value)
            {
                ret++;
            }
        }

        return ret;
    }

    public boolean isTrump()
    {
        return m_Trump;
    }

    public void setTrump(boolean _trump)
    {
        m_Trump = _trump;
    }
    
    public String colorName()
    {
        String name="";
        switch (m_Color)
        {
            case COEUR:
                name += "Coeur";
                break;
            case CARREAU:
                name += "Carreau";
                break;
            case TREFLE:
                name += "Trèfle";
                break;
            case PIC:
                name += "Pic";
                break;
            default:
                name += "ERROR";
        }
        return name;
    }

    public String displayName()
    {
        String name;

        switch (m_Value)
        {
            case SEPT:
                name = "7 ";
                break;
            case HUIT:
                name = "8 ";
                break;
            case NEUF:
                name = "9 ";
                break;
            case DIX:
                name = "10 ";
                break;
            case VALET:
                name = "V ";
                break;
            case DAME:
                name = "D ";
                break;
            case ROI:
                name = "R ";
                break;
            case AS:
                name = "As ";
                break;
            default:
                name = "ERROR";
                break;
        }

        name += colorName();

        return name;
    }
}
