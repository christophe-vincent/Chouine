/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package chouinetest;

import Cards.Announce;
import ChouineAlgo.Chouine;
import Tools.Tracer;
import java.security.NoSuchAlgorithmException;
import java.util.List;

/**
 *
 * @author vincentc
 */
public class ChouineTest
{

    public static final Tracer.Verbosity VERBOSE_LEVEL = Tracer.Verbosity.NONE;
    public static final int NUMBER_OF_GAMES = 1000000;
    
    public static final int PLAYER1_ALGO = 0; 
    public static final int PLAYER1_LEVEL = 5; 
    
    public static final int PLAYER2_ALGO = 0;  
    public static final int PLAYER2_LEVEL = 6;  
    
    public static final boolean DISPLAY_POINTS = false;

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) throws NoSuchAlgorithmException
    {
        int[] playerScore;
        String[] playerName =
        {
            "Player A", "player B"
        };
        int[] playerLevel =
        {
            PLAYER1_LEVEL, PLAYER2_LEVEL
        };
        int[] playerAlgo =
        {
            PLAYER1_ALGO, PLAYER2_ALGO
        };
        int winner;
        float pc;

        playerScore = new int[2];
        playerScore[0] = 0;
        playerScore[1] = 0;

        for (int i = 0; i < NUMBER_OF_GAMES; i++)
        {
            if ((i % 10000) == 0)
            {
                System.out.print(String.valueOf(i) + "/" + NUMBER_OF_GAMES + "");
                System.out.println(" - " + String.valueOf(100 * i / NUMBER_OF_GAMES) + "%");
            }
            //System.out.println("Game " + String.valueOf(i) + ":");
            winner = game(playerAlgo[0], playerLevel[0], playerAlgo[1], playerLevel[1]);
            playerScore[winner]++;
        }

        System.out.println("--------------------------------");
        System.out.println("           RESULTS ");
        System.out.println(String.valueOf(NUMBER_OF_GAMES) + " parties");
        pc = 0;
        for (int i = 0; i < 2; i++)
        {
            System.out.print(playerName[i] + " (algo ");
            System.out.print(String.valueOf(playerAlgo[i]) + ", level " + String.valueOf(playerLevel[i]) + ") : ");
            System.out.print(String.valueOf(playerScore[i]));
            pc = (float) (100 * playerScore[i]) / (float) NUMBER_OF_GAMES;
            System.out.printf(" - %.2f", pc);
            System.out.println("%");
        }
        System.out.println("Différentiel : " + String.valueOf(Math.abs((50 - pc) * 2)));
    }

    static int game(int _algo1, int _level1, int _algo2, int _level2) throws NoSuchAlgorithmException
    {
        Chouine game;
        int winner;
        List<Announce> announces;
        int[] points = new int[2];
        points[0] = 0;
        points[1] = 0;

        game = new Chouine();
        game.setVerbosity(VERBOSE_LEVEL);
        game.setPlayerLevel(0, _level1);
        game.setPlayerLevel(1, _level2);
        game.setPlayerAlgo(0, _algo1);
        game.setPlayerAlgo(1, _algo2);
        
        game.NewGame("Joueur A", "Joueur B");
        
        game.DistributeCards(true); // use random to choose who starts the game
        Announce ann;
        /*
        game.testBruteForce();
        return 0;*/

        do
        {
            game.getPlayerChoice(game.donnor(), false);
            ann = game.getPlayerLatestAnnounce(game.donnor());
            if (ann != null)
            {
                //System.out.println("J1 : " + ann.getName() + ", " + String.valueOf(ann.getPoints()) + " points");
            }
            game.getPlayerChoice((game.donnor() + 1) % 2, false);
            ann = game.getPlayerLatestAnnounce((game.donnor() + 1) % 2);
            if (ann != null)
            {
                //System.out.println("J2 : " + ann.getName() + ", " + String.valueOf(ann.getPoints()) + " points");
            }

            winner = game.Play();
        } while (!game.isGameOver());

        if (DISPLAY_POINTS)
        {
            for (int i = 0; i < 2; i++)
            {
                points[i] = game.getPlayerPoints(i);
                System.out.print(game.getPlayerName(i) + " : " + String.valueOf(points));
                if (game.getPlayer10Der(i) > 0)
                {
                    points[i] += game.getPlayer10Der(i);
                    System.out.print(" + " + String.valueOf(game.getPlayer10Der(i)));
                }
                announces = game.getPlayerAnnouces(i);
                if (!announces.isEmpty())
                {
                    for (Announce an : announces)
                    {
                        points[i] += an.getPoints();
                        System.out.print(" + " + String.valueOf(an.getPoints()));
                    }
                }

                if (points[i] != game.getPlayerPoints(i))
                {
                    System.out.print(" = " + String.valueOf(points));
                }
                System.out.println("");
            }
            System.out.println("");
        }

        for (int i = 0; i < 2; i++)
        {
            points[i] = game.getPlayerPoints(i);
            if (game.getPlayer10Der(i) > 0)
            {
                points[i] += game.getPlayer10Der(i);
            }
            announces = game.getPlayerAnnouces(i);
            if (!announces.isEmpty())
            {
                for (Announce an : announces)
                {
                    points[i] += an.getPoints();
                }
            }
        }

        winner = 0;
        if (points[1] > points[0])
        {
            winner = 1;
        }

        return winner;
        
    }
}
