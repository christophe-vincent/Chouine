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
public class Announce implements Cloneable
{

    static public final int CARD_POINT = 10;
    static public final Card.CardValue[] MARIAGE_CARDS =
    {
        Card.CardValue.DAME, Card.CardValue.ROI
    };
    static public final Card.CardValue[] TIERCE_CARDS =
    {
        Card.CardValue.VALET, Card.CardValue.DAME, Card.CardValue.ROI,
    };
    static public final Card.CardValue[] QUARANTE_CARDS =
    {
        Card.CardValue.ROI, Card.CardValue.DAME,
        Card.CardValue.VALET, Card.CardValue.AS
    };
    static public final Card.CardValue[] CHOUINE_CARDS =
    {
        Card.CardValue.ROI, Card.CardValue.DAME,
        Card.CardValue.VALET, Card.CardValue.DIX,
        Card.CardValue.AS
    };

    public enum AnnounceType
    {

        NONE(0), MARIAGE(20), TIERCE(30), QUARANTE(40), QUINTE(50), CHOUINE(1000);
        private final int value;

        private AnnounceType(int value)
        {
            this.value = value;
        }

        public int getValue()
        {
            return value;
        }
    };
    private List<Card> m_Cards;
    private Card.Color m_Color;
    private AnnounceType m_Announce;
    private int m_Points;
    private boolean m_IsTrump;
    private float m_Score;

    Announce(Card.Color _color, AnnounceType _type, boolean _trump, List<Card> _cards)
    {
        m_Color = _color;
        m_Announce = _type;
        m_IsTrump = _trump;
        m_Cards = _cards;

        computeScore();
    }
    
    Announce(Card.Color _color, AnnounceType _type, boolean _trump, Card _card)
    {
        m_Color = _color;
        m_Announce = _type;
        m_IsTrump = _trump;
        m_Cards = new LinkedList();
        m_Cards.add(_card);

        computeScore();
    }

    private void computeScore()
    {
        // score depends on the card number
        switch (m_Announce)
        {

            case MARIAGE:
                m_Score = (float) m_Cards.size() / (float) MARIAGE_CARDS.length;
                break;
            case TIERCE:
                m_Score = (float) m_Cards.size() / (float) TIERCE_CARDS.length;
                break;
            case QUARANTE:
                m_Score = (float) m_Cards.size() / (float) QUARANTE_CARDS.length;
                break;
            case QUINTE:
                m_Score = (float) m_Cards.size() / 5.0f;
                break;
            case CHOUINE:
                m_Score = (float) m_Cards.size() / (float) CHOUINE_CARDS.length;
                break;
        }
    }

    public Object clone()
    {
        Object o = null;
        try
        {
            // On récupère l'instance à renvoyer par l'appel de la 
            // méthode super.clone()
            o = super.clone();
        } catch (CloneNotSupportedException cnse)
        {
            // Ne devrait jamais arriver car nous implémentons 
            // l'interface Cloneable
            cnse.printStackTrace(System.err);
        }
        // on renvoie le clone
        return o;
    }

    public AnnounceType getType()
    {
        return m_Announce;
    }

    public boolean isTrump()
    {
        return m_IsTrump;
    }

    public int getPoints()
    {
        if (m_IsTrump)
        {
            return 2 * m_Announce.getValue();
        } else
        {
            return m_Announce.getValue();
        }
    }

    public Card.Color getColor()
    {
        return m_Color;
    }

    public String getName()
    {
        String ret = "";
        switch (m_Announce)
        {
            case CHOUINE:
                ret = "Chouine";
                break;
            case MARIAGE:
                ret = "Mariage";
                break;
            case NONE:
                ret = "Erreur";
                break;
            case QUARANTE:
                ret = "Quarante";
                break;
            case QUINTE:
                ret = "Quinte";
                break;
            case TIERCE:
                ret = "Tierce";
                break;
        }
        return ret;
    }
    
    public float getScore()
    {
        return m_Score;
    }

    public boolean containsCard(Card _card)
    {
        return m_Cards.contains(_card);
    }

    public void addCard(Card _card)
    {
        if (!m_Cards.contains(_card))
        {
            m_Cards.add(_card);
            // increase the score
            computeScore();
        }
    }
    
    public void removeCard(Card _card)
    {
        if (m_Cards.contains(_card))
        {
            m_Cards.remove(_card);
            // increase the score
            computeScore();
        }

    }

    public static List<Announce> isAnnounce(CardList _list, List<Announce> _knownAnnounces)
    {
        Announce ann;
        CardList colorList;
        CardList cards;
        List<Card.Color> colorToCheck;

        List<Announce> ret = new LinkedList();
        if (_list.size() <= 1)
        {
            return ret;
        }

        if (isQuinte(_list) == AnnounceType.QUINTE.value)
        {
            // if quinte already announce, do not accep it
            boolean quintePresent = false;
            for (Announce ca : _knownAnnounces)
            {
                if (ca.getType() == AnnounceType.QUINTE)
                {
                    quintePresent = true;
                }
            }
            if (!quintePresent)
            {
                ann = new Announce(Card.Color.BACK, AnnounceType.QUINTE, false, _list);
                ret.add(ann);
            }
            return ret;
        }

        // remove the color that have already been annouced
        // first init a list with all colors
        colorToCheck = new LinkedList();
        for (int i = 0; i < Card.ALL_COLORS.length; i++)
        {
            colorToCheck.add(Card.ALL_COLORS[i]);
        }

        // remove colors already announced
        for (Announce an : _knownAnnounces)
        {
            if (colorToCheck.contains(an.getColor()))
            {
                colorToCheck.remove(an.getColor());
            }
        }


        for (Card.Color color : colorToCheck)
        {

            colorList = _list.getColorSubset(color);
            if (isChouine(colorList) == 50)
            {
                ann = new Announce(color, AnnounceType.CHOUINE,
                        _list.get(0).isTrump(), _list);
                ret.add(ann);
            } else if (isQuarante(colorList) == AnnounceType.QUARANTE.value)
            {
                cards = new CardList();

                for (Card.CardValue cv : QUARANTE_CARDS)
                {
                    cards.add(colorList.searchCard(cv));
                }

                ann = new Announce(color, AnnounceType.QUARANTE,
                        _list.get(0).isTrump(), cards);
                ret.add(ann);
            } else if (isTierce(colorList) == AnnounceType.TIERCE.value)
            {
                cards = new CardList();
                for (Card.CardValue cv : TIERCE_CARDS)
                {
                    cards.add(colorList.searchCard(cv));
                }

                ann = new Announce(color, AnnounceType.TIERCE,
                        _list.get(0).isTrump(), cards);
                ret.add(ann);
            } else if (isMariage(colorList) == AnnounceType.MARIAGE.value)
            {
                cards = new CardList();
                for (Card.CardValue cv : MARIAGE_CARDS)
                {
                    cards.add(colorList.searchCard(cv));
                }
                ann = new Announce(color, AnnounceType.MARIAGE,
                        _list.get(0).isTrump(), cards);
                ret.add(ann);
            }
        }

        return ret;
    }

    static int isMariage(List<Card> _list)
    {
        int points = 0;

        for (Card.CardValue v : MARIAGE_CARDS)
        {
            if (Card.containsValue(v, _list))
            {
                points += 10;
            }
        }

        return points;
    }

    static int isTierce(List<Card> _list)
    {
        int points = 0;

        for (Card.CardValue v : TIERCE_CARDS)
        {
            if (Card.containsValue(v, _list))
            {
                points += 10;
            }
        }

        return points;
    }

    static int isQuarante(List<Card> _list)
    {
        int points = 0;

        for (Card.CardValue v : QUARANTE_CARDS)
        {
            if (Card.containsValue(v, _list))
            {
                points += 10;
            }
        }

        return points;
    }

    static int isChouine(List<Card> _list)
    {
        int points = 0;

        for (Card.CardValue v : CHOUINE_CARDS)
        {
            if (Card.containsValue(v, _list))
            {
                points += 10;
            }
        }

        return points;
    }

    static int isQuinte(List<Card> _list)
    {
        int n10, nAs;

        n10 = Card.NumberOf(Card.CardValue.AS, _list);
        nAs = Card.NumberOf(Card.CardValue.DIX, _list);

        return 10*(n10 + nAs);
    }
}
