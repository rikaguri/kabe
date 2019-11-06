using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;


public class changetokuten : MonoBehaviour
{
    [SerializeField] float playTime         = 0f;
    [SerializeField] GameObject _fadePanael = null;

    // Start is called before the first frame update
    void Start()
    {
        Invoke("SceneChange", playTime);
    }

    void Update()
    {
        if (_fadePanael.GetComponent<FadeScript>().GetFadeState() == FadeScript.FadeState.FADE_OUT_COMPRETED)
        {
            SceneManager.LoadScene("tokuten");
        }
    }

    private void SceneChange()
    {
        _fadePanael.GetComponent<FadeScript>().StartFadeOut();
    }

}
