/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package ChouineRenderer;

import com.jme3.math.ColorRGBA;
import com.jme3.math.Vector3f;

/**
 *
 * @author vincentc
 */
public class Positions
{
    public static final float SCALE = 4;
    
    // cam
    public static final float CAM_X = SCALE*(-0.03f);
    public static final float CAM_Y = SCALE*0;
    public static final float CAM_Z = SCALE*0.40f;
    public static final float CAM_ANGLE = -0.0f;
    public static final Vector3f CAM_LOOKAT = new Vector3f(
            SCALE*-0.03f, 0, 0);
    
    
    // playing card limit
    public static final float CARD_ISPLAYED_LIMIT_Y = SCALE*-0.10f;
    
    // motions
    public static final float ROTATION_SEEP = 0.05f;
    public static final float ZOOM_Y_MAX = SCALE*0.1f;
    public static final float ZOOM_Y_MIN = SCALE*3;
    public static final float ZOOM_SPEED = SCALE*0.05f;
    
    // carpet
    public static final float CARPET_HEIGHT = SCALE*0.50f;
    public static final float CARPET_WIDTH = SCALE*0.60f;
    public static final float CARPET_TICK = SCALE*0.005f;
    public static final Vector3f CARPET_POS = new Vector3f(0, 0, (SCALE*-0.01f)-CARPET_TICK/2);
    public static final ColorRGBA CARPET_COLOR = new ColorRGBA(0,0.8f,0,1f);
    
    // cards
    public static final float CARD_HEIGHT = SCALE*0.087f; // 8.7cm
    public static final float CARD_WIDTH = SCALE*0.087f*(209.0f / 303.0f);
    public static final float CARD_TICK = SCALE*0.0005f;
    
    public static final Vector3f PICK_POS = new Vector3f(
            SCALE*0.15f, -CARD_HEIGHT/2, Positions.CARD_TICK/2);
    
    public static final Vector3f COMPUTER_PLAYED_POS = new Vector3f(
            SCALE*-0.03f, -CARD_HEIGHT/2, Positions.CARD_TICK/2);
    
    public static final Vector3f USER_CARD1_POS = new Vector3f(
            SCALE*-0.19f, SCALE*-0.16f, Positions.CARD_TICK/2);
    
    public static final Vector3f COMPUTER_CARD1_POS = new Vector3f(
            SCALE*-0.19f, SCALE*0.07f, Positions.CARD_TICK/2);
    
    public static final float CARD_SPACING = SCALE*0.062f;
    
    public static final Vector3f USER_WINCARDS_POS = new Vector3f(
            SCALE*0.15f, SCALE*-0.16f, Positions.CARD_TICK/2);
    
    public static final Vector3f COMPUTER_WINCARDS_POS = new Vector3f(
            SCALE*0.15f, SCALE*0.06f, Positions.CARD_TICK/2);
    
    // cards shown at the end of the game
    public static final Vector3f COMPUTER_ALLCARDS_POS = new Vector3f(
            SCALE*-0.19f, SCALE*0.07f, Positions.CARD_TICK/2);
    
    public static final Vector3f USER_ALLCARDS_POS = new Vector3f(
            SCALE*-0.19f, SCALE*-0.16f, Positions.CARD_TICK/2);
    
    // motions
    public static final Vector3f ANNOUNCE_MOTION = new Vector3f(
            0, SCALE*(0.05f), SCALE*(0.05f));
    
}
