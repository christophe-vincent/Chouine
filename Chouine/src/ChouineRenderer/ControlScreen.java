package ChouineRenderer;

import Cards.Announce;
import ChouineAlgo.Chouine;
import ChouineRenderer.Main.STATE;
import com.jme3.app.Application;
import com.jme3.app.SimpleApplication;
import com.jme3.app.state.AbstractAppState;
import com.jme3.app.state.AppStateManager;
import de.lessvoid.nifty.EndNotify;
import de.lessvoid.nifty.Nifty;
import de.lessvoid.nifty.NiftyEventSubscriber;
import de.lessvoid.nifty.controls.Button;
import de.lessvoid.nifty.controls.CheckBox;
import de.lessvoid.nifty.controls.RadioButton;
import de.lessvoid.nifty.controls.RadioButtonGroupStateChangedEvent;
import de.lessvoid.nifty.controls.SliderChangedEvent;
import de.lessvoid.nifty.controls.button.ButtonControl;
import de.lessvoid.nifty.controls.slider.SliderControl;
import de.lessvoid.nifty.effects.EffectEventId;
import de.lessvoid.nifty.elements.Element;
import de.lessvoid.nifty.elements.render.TextRenderer;
import de.lessvoid.nifty.screen.Screen;
import de.lessvoid.nifty.screen.ScreenController;
import de.lessvoid.nifty.tools.Color;
import java.security.NoSuchAlgorithmException;
import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author vincentc
 */
public class ControlScreen extends AbstractAppState implements ScreenController
{

    private Nifty nifty;
    private Screen m_StartScreen;
    private Screen m_GameScreen;
    private SimpleApplication app;
    private Main m_Main;
    CardManager m_CardManager;
    Chouine m_Chouine;
    private Element m_MessageLayer;
    private Element m_MessageText;
    Match m_CurrentMatch;
    private Element m_PopupConfirmLeaveRound;

    /**
     * custom methods
     */
    public ControlScreen(Main _main, CardManager _cardMgr, Chouine _chouine)
    {
        m_Main = _main;
        m_CardManager = _cardMgr;
        m_Chouine = _chouine;
    }

    /**
     * Nifty GUI ScreenControl methods
     */
    public void bind(Nifty nifty, Screen screen)
    {
        this.nifty = nifty;
        if (screen.getScreenId().equals("game"))
        {
            m_MessageLayer = screen.findElementByName("messagelayer");
            m_MessageText = screen.findElementByName("messagetext");
            m_GameScreen = screen;
        } else
        {
            m_StartScreen = screen;
            SliderControl slider = screen.findNiftyControl("sliderTempo", SliderControl.class);
            if (slider != null)
            {
                slider.setMin(0.5f);
                slider.setMax(4.0f);
                slider.setStepSize(0.5f);
                slider.setButtonStepSize(0.5f);
                slider.setValue(Options.getRemoveCardsTimeOut());
            }

            RadioButton rb = screen.findNiftyControl("cl_high", RadioButton.class);
            rb.select();
        }

    }

    public void onStartScreen()
    {
        if (nifty.getCurrentScreen().getScreenId().equals("game"))
        {
            if (m_MessageLayer != null)
            {
                m_MessageLayer.setVisible(false);
            }
            if (m_MessageText != null)
            {
                m_MessageText.setVisible(false);
            }
            m_PopupConfirmLeaveRound = nifty.createPopup("popupConfirm");
            Element layer = m_GameScreen.findElementByName("controls");
            layer.startEffect(EffectEventId.onCustom, new FadeInControlEnd(), "fadeInControls");
            m_GameScreen.findElementByName("gameoverlayer").setVisible(false);
        }
    }

    public void onEndScreen()
    {
    }

    /**
     * jME3 AppState methods
     */
    @Override
    public void initialize(AppStateManager stateManager, Application app)
    {
        super.initialize(stateManager, app);
        this.app = (SimpleApplication) app;
    }

    @Override
    public void update(float tpf)
    {
        /**
         * jME update loop!
         */
    }

    public void startGame() throws InterruptedException
    {
        nifty.gotoScreen("game");
        CheckBox cb = m_StartScreen.findNiftyControl("CheckBoxHelp", CheckBox.class);
        Options.setShowCardToPlay(cb.isChecked());
        m_CurrentMatch = new Match(getWinPoints(), getWinRounds());
        updateMatchPoints();
        updateComputerScore(0, 0);
        updateUserScore(0, 0);
    }

    public void quitGame()
    {
        System.exit(0);
    }

    public void confirmOK()
    {
        m_Main.clear();
        nifty.closePopup(m_PopupConfirmLeaveRound.getId());
        nifty.gotoScreen("start");
    }

    public void confirmKO()
    {
        nifty.closePopup(m_PopupConfirmLeaveRound.getId());
    }

    private int getWinPoints()
    {
        int points = 0;
        RadioButton rb = m_StartScreen.findNiftyControl("1point", RadioButton.class);
        if (rb.isActivated())
        {
            points = 1;
        }

        rb = m_StartScreen.findNiftyControl("3points", RadioButton.class);
        if (rb.isActivated())
        {
            points = 3;
        }

        rb = m_StartScreen.findNiftyControl("5points", RadioButton.class);
        if (rb.isActivated())
        {
            points = 3;
        }
        return points;
    }

    private int getWinRounds()
    {
        int points = 0;
        RadioButton rb = m_StartScreen.findNiftyControl("1round", RadioButton.class);
        if (rb.isActivated())
        {
            points = 1;
        }

        rb = m_StartScreen.findNiftyControl("2round", RadioButton.class);
        if (rb.isActivated())
        {
            points = 2;
        }

        return points;
    }

    @NiftyEventSubscriber(id = "sliderTempo")
    public void onSliderChange(String id, SliderChangedEvent event)
    {
        Element elem = m_StartScreen.findElementByName("tempoVal");
        elem.getRenderer(TextRenderer.class).setText("   " + event.getValue() + " sec");
        Options.setRemoveCardsTimeOut(event.getValue());
    }

    @NiftyEventSubscriber(id = "RadioGroup-3")
    public void onRadioGroup1Changed(final String id, final RadioButtonGroupStateChangedEvent event)
    {
        Options.setComputerLevel(event.getSelectedId());
    }

    public void startAnnounce()
    {
        if (m_Main.getState() == STATE.USER_ANNOUNCE)
        {
            List<String> list;
            int points;
            String message;

            m_GameScreen.findControl("AnnounceButton", ButtonControl.class).setText("Annonce");
            list = m_CardManager.endAnnouce();
            Announce ann;
            ann = m_Chouine.setPlayerAnnounce(CardManager.PLAYER_ID, list);
            if (ann != null)
            {
                points = ann.getPoints();
                message = ann.getName();
                if (ann.isTrump())
                {
                    message += " d'atout";
                }
                if (ann.getType() != Announce.AnnounceType.CHOUINE)
                {
                    message += " : " + String.valueOf(points) + " points";
                }
            } else
            {
                message = "Cette annonce n'est pas valide...";
            }

            updateUserScore(
                    m_Chouine.getPlayerPoints(CardManager.PLAYER_ID),
                    m_Chouine.getPlayerAnnouncesPoints(CardManager.PLAYER_ID));
            newMessage(message);

            m_Main.setState(STATE.PLAY);
            if (m_Chouine.isGameOver())
            {
                m_Main.clear();
                m_Main.displayResults();
            }
        } else
        {
            newMessage("Cliquez sur les cartes, puis sur  'valider'");
            m_GameScreen.findControl("AnnounceButton", ButtonControl.class).setText("Valider");

            m_Main.setState(STATE.USER_ANNOUNCE);
        }
    }

    public void trumpCardName(String _name)
    {
        if (m_GameScreen.findElementByName("trump") != null)
        {
            m_GameScreen.findElementByName("trump").getRenderer(TextRenderer.class).setText(_name);
        }
    }

    public void changeTrumpCard()
    {
        String name;
        name = m_Chouine.change7Trump(CardManager.PLAYER_ID);
        if (name != null)
        {
            m_CardManager.exchangeTrumpCard(CardManager.PLAYER_ID, name);
        }
    }

    public void changePickCardLeft(int _number)
    {
        if (m_GameScreen.findElementByName("pickNumber") != null)
        {
            m_GameScreen.findElementByName("pickNumber").getRenderer(TextRenderer.class).setText(String.valueOf(_number) + " cartes");
        }
    }

    public void newMessage(String _message)
    {
        if (m_MessageLayer != null)
        {
            m_MessageLayer.setVisible(true);
            m_MessageText.getRenderer(TextRenderer.class).setText(_message);
            m_MessageLayer.startEffect(EffectEventId.onCustom, new FadeOutEnd(), "fadeOut1");
        }
    }

    public void updateComputerScore(int _points, int _announces)
    {
        m_GameScreen.findElementByName("computerScorePoints").getRenderer(TextRenderer.class).setText(" " + String.valueOf(_points));
        m_GameScreen.findElementByName("computerScoreAnn").getRenderer(TextRenderer.class).setText(" " + String.valueOf(_announces));
    }

    public void updateUserScore(int _points, int _announces)
    {
        m_GameScreen.findElementByName("userScorePoints").getRenderer(TextRenderer.class).setText(" " + String.valueOf(_points));
        m_GameScreen.findElementByName("userScoreAnn").getRenderer(TextRenderer.class).setText(" " + String.valueOf(_announces));
    }

    public void updateMatchPoints()
    {
        int wp = m_CurrentMatch.getWinPoints();
        int wr = m_CurrentMatch.getWinRonds();

        int val = m_CurrentMatch.getUserPoints();
        String msg = " " + String.valueOf(val) + " / " + String.valueOf(wp);
        m_GameScreen.findElementByName("userRoundPoints").getRenderer(TextRenderer.class).setText(msg);

        val = m_CurrentMatch.getUserRounds();
        msg = " " + String.valueOf(val) + " / " + String.valueOf(wr);
        m_GameScreen.findElementByName("userRounds").getRenderer(TextRenderer.class).setText(msg);

        val = m_CurrentMatch.getComputerPoints();
        msg = " " + String.valueOf(val) + " / " + String.valueOf(wp);
        m_GameScreen.findElementByName("computerRoundPoints").getRenderer(TextRenderer.class).setText(msg);

        val = m_CurrentMatch.getComputerRounds();
        msg = " " + String.valueOf(val) + " / " + String.valueOf(wr);
        m_GameScreen.findElementByName("computerRounds").getRenderer(TextRenderer.class).setText(msg);
    }

    
    public void gameOverScreen(int _userPoints, int _computerPoints)
    {
        Color color;
        Match.Gain gain;
        String msg;
        boolean endMatch = false;
        // prepares the message
        if (_computerPoints < _userPoints)
        {
            msg = "Vous avez gagné cette partie\n";
            if ((_userPoints < 1000) && (_computerPoints < 1000))
            {
                msg += String.valueOf(_userPoints);
                msg += " contre " + String.valueOf(_computerPoints);
            }
            color = new Color(0, 0, 1, 1);

            gain = m_CurrentMatch.newUserPoint();
            if (gain == Match.Gain.ROUND)
            {
                msg += "\nVous remportez cette manche !";
            } else if (gain == Match.Gain.MATCH)
            {
                msg += "\nBravo, vous remportez le match !!!";
                endMatch = true;
            } else
            {
                msg = "\n" + msg;
            }
        } else
        {
            if ((_userPoints < 1000) && (_computerPoints < 1000))
            {
                msg = "Vous avez perdu cette partie\n";
                msg += String.valueOf(_computerPoints);
                msg += " contre " + String.valueOf(_userPoints);
            }
            else
            {
                msg  = "Ordinateur : Chouine !\n";
                msg += "Vous avez perdu cette partie";
            }
            color = new Color(1, 0, 0, 1);
 
            gain = m_CurrentMatch.newComputerPoint();
            if (gain == Match.Gain.ROUND)
            {
                msg += "\nL'ordinateur remporte cette manche !";
            } else if (gain == Match.Gain.MATCH)
            {
                msg += "\nPerdu, vous ne gagnez pas le match...";
                endMatch = false;
            } else
            {
                msg = "\n" + msg;
            }
        }

        updateMatchPoints();

        m_GameScreen.findElementByName("gameoverlayer").setVisible(true);
        m_GameScreen.findElementByName("gameovertext").getRenderer(TextRenderer.class).setText(msg);

        m_GameScreen.findElementByName("gameovertext").getRenderer(TextRenderer.class).setColor(color);
        if (endMatch)
        {
            m_GameScreen.findControl("stopPlayButton", ButtonControl.class).disable();
        }
    }
    
    public void error(String message)
    {
        m_GameScreen.findElementByName("gameoverlayer").setVisible(true);
        m_GameScreen.findElementByName("gameovertext").getRenderer(TextRenderer.class).setText(message);

        
    }

    public void replay() throws NoSuchAlgorithmException
    {
        m_GameScreen.findElementByName("gameoverlayer").setVisible(false);

        if (m_CurrentMatch.isMatchOver())
        {
            m_Main.clear();
            nifty.gotoScreen("start");
        } else
        {
            m_Main.initGame();
        }
    }

    public void finishRound()
    {
        Element focus = m_PopupConfirmLeaveRound.findElementByName("buttonConfirmKo");
        nifty.showPopup(nifty.getCurrentScreen(), m_PopupConfirmLeaveRound.getId(), focus);
    }

    public void stopPlaying()
    {
        Element focus = m_PopupConfirmLeaveRound.findElementByName("buttonConfirmKo");
        nifty.showPopup(nifty.getCurrentScreen(), m_PopupConfirmLeaveRound.getId(), focus);
    }

    class FadeOutEnd implements EndNotify
    {

        @Override
        public void perform()
        {
            m_MessageLayer.startEffect(EffectEventId.onCustom, null, "fadeOut2");
        }
    }

    class FadeInControlEnd implements EndNotify
    {

        @Override
        public void perform()
        {
            try
            {
                m_Main.initGame();
            } catch (NoSuchAlgorithmException ex)
            {
                Logger.getLogger(ControlScreen.class.getName()).log(Level.SEVERE, null, ex);
            }
        }
    }
}
