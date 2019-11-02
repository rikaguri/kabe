using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class FadeScript : MonoBehaviour
{
    float alfa;
    float speed = 0.01f;
    float red = 0, green = 0, blue = 0;

    [SerializeField] float _fadeOutTime, _fadeInTime;

    public enum FadeState
    {
        NONE,
        FADE_OUT,
        FADE_OUT_COMPRETED,
        FADE_IN,
    };

    FadeState _fadeState;


    void Start()
    {
        //_fadeState = FadeState.NONE;
        //red = GetComponent<Image>().color.r;
        //green = GetComponent<Image>().color.g;
        //blue = GetComponent<Image>().color.b;

    }

    void Update()
    {
        if (Input.GetKeyDown(KeyCode.F))
            StartFadeOut();

        if (_fadeState == FadeState.NONE)
            return;

        switch (_fadeState)
        {
            case FadeState.FADE_OUT:
                alfa += Time.deltaTime / _fadeOutTime;
                if (alfa >= 1)
                {
                    alfa = 1;
                    _fadeState = FadeState.FADE_OUT_COMPRETED;
                }
                break;

            case FadeState.FADE_OUT_COMPRETED:
                _fadeState = FadeState.FADE_IN;
                break;

            case FadeState.FADE_IN:
                alfa -= Time.deltaTime / _fadeInTime;
                if (alfa <= 0)
                {
                    alfa = 0;
                    _fadeState = FadeState.NONE;
                    gameObject.SetActive(false);
                }
                break;

        }

        GetComponent<Image>().color = new Color(red, green, blue, alfa);

    }

    public void StartFadeOut()
    {
        if (_fadeState != FadeState.NONE)
            return;

        _fadeState = FadeState.FADE_OUT;

        gameObject.SetActive(true);

    }

    public void SetColor(float r, float g, float b)
    {
        red = r;
        green = g;
        blue = b;
    }

    public FadeState GetFadeState()
    {
        return _fadeState;
    }





}
