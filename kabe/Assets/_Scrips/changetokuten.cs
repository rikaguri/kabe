using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;


public class changetokuten : MonoBehaviour
{
    [SerializeField] float playTime         = 0f;
    [SerializeField] GameObject _fadePanael = null;

	public SerialHandler serialHandler;

	// Start is called before the first frame update
	void Start()
    {
		//Invoke("SceneChange", playTime);
		serialHandler.OnDataReceived += OnDataReceived;
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

	void OnDataReceived(string message)
	{
		var data = message.Split(
				new string[] { "\t" }, System.StringSplitOptions.None);
		if (data.Length < 2) return;
		try
		{
			ScoreManager.blueScore = int.Parse(data[0]);
			ScoreManager.pinkScore = int.Parse(data[1]);
			SceneChange();
		}
		catch (System.Exception e)
		{
			Debug.LogWarning(e.Message);
		}
	}
}
