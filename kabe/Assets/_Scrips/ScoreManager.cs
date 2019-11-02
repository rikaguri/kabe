using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using TMPro;

public class ScoreManager : MonoBehaviour
{
    [SerializeField] TextMeshProUGUI pinkScoreText      = null;
    [SerializeField] TextMeshProUGUI blueScoreText      = null;
    [SerializeField] TextMeshProUGUI topScoreText       = null;
    [SerializeField] Image pinkWinBanner                = null;
    [SerializeField] Image blueWinBanner                = null;
    [SerializeField] Image pinkLoseBanner               = null;
    [SerializeField] Image blueLoseBanner               = null;
    [SerializeField] Image updateBanner                 = null;
    [SerializeField] GameObject pinkParticle            = null;
    [SerializeField] GameObject blueParticle            = null;

    [SerializeField] bool resetFlag                     = true;

    private int pinkScore                               = 0;
    private int blueScore                               = 0;
    private int topScore                                = 0;
    private int topScorePreserved                       = 0;


    void Init()
    {
        PlayerPrefs.SetInt("TopScore", 0);
        pinkScore = 0;
        blueScore = 0;
    }

    void uiInit() 
    {
        pinkWinBanner.gameObject.SetActive(false);
        blueWinBanner.gameObject.SetActive(false);
        pinkLoseBanner.gameObject.SetActive(false);
        blueLoseBanner.gameObject.SetActive(false);
        updateBanner.gameObject.SetActive(false);
        pinkParticle.SetActive(false);
        blueParticle.SetActive(false);

        pinkScoreText.text = "000";
        blueScoreText.text = "000";
        topScoreText.text  = "000";
    }

    // Start is called before the first frame update
    void Start()
    {
        //値をリセットしたい時はインスペクター上でresetFlagをtrueにしてください。
        if(resetFlag){
            Init();
        }

        //情報があるかどうかのチェック
        if (!PlayerPrefs.HasKey("TopScore")){
            PlayerPrefs.SetInt("TopScore", 0);
        }

        uiInit();
        getTopScore();

        //topscoreに更新がなければTopScore表示
        if (!updateTopScore())
        {
            topScoreText.text = topScore.ToString("000");
        }

        StartCoroutine(viewResult());
    }

    IEnumerator viewResult()
    {

        //blueスコア表示アニメーション
        for (int _blueScore = 0; _blueScore <= blueScore; _blueScore += 5)
        {
            blueScoreText.text = _blueScore.ToString("000");
            yield return new WaitForSeconds(0.001f);
        }
        blueScoreText.text = blueScore.ToString("000");

        //pinkスコア表示アニメーション
        for (int _pinkScore = 0; _pinkScore <= pinkScore; _pinkScore += 5)
        {
            pinkScoreText.text = _pinkScore.ToString("000");
            yield return new WaitForSeconds(0.001f);
        }
        pinkScoreText.text = pinkScore.ToString("000");

        yield return new WaitForSeconds(1.0f);

        //win lose バナー表示アニメーション
        setWinAndLoseBanner();

        yield return new WaitForSeconds(3.0f);


        //topscoreに更新があればTopScore表示アニメーション
        if (updateTopScore()){

            for (int _topScore = 0; _topScore <= topScore; _topScore += 5)
            {
                topScoreText.text = _topScore.ToString("000");
                yield return new WaitForSeconds(0.0001f);
            }
            topScoreText.text = topScore.ToString("000");

            yield return new WaitForSeconds(0.5f);
            updateBanner.gameObject.SetActive(true);

        }

    }

    private void setWinAndLoseBanner() {
        if(pinkScore > blueScore) {
            pinkWinBanner.gameObject.SetActive(true);
            blueLoseBanner.gameObject.SetActive(true);
            pinkParticle.SetActive(true);
        }
        else if(blueScore > pinkScore){
            blueWinBanner.gameObject.SetActive(true);
            pinkLoseBanner.gameObject.SetActive(true);
            blueParticle.SetActive(true);
        }
    }

    private int getTopScore() 
    {
        topScorePreserved = PlayerPrefs.GetInt("TopScore", 0);
        List<int> scoreList = new List<int>();
        scoreList.Add(pinkScore);
        scoreList.Add(blueScore);
        scoreList.Add(topScorePreserved);
        scoreList.Sort((lhs, rhs) => rhs - lhs);
        topScore = scoreList[0];

        return topScore;
    }

    private bool updateTopScore() 
    {
        if(topScorePreserved < topScore) {
            PlayerPrefs.SetInt("TopScore", topScore);
            return true;
        }else{
            return false;
        }
    }
}
