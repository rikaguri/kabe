using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;


public class changetokuten : MonoBehaviour
{
    [SerializeField] float playTime         = 0f;
    [SerializeField] GameObject _fadePanael = null;

	public SerialHandler serialHandler;
	public string finish =null;

	// Start is called before the first frame update
	void Start()
    {
		serialHandler.OnDataReceived += OnDataReceived;
		if (finish == "fin")
		{
			SceneChange();
		}
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

			finish = data[0];
		}
		catch (System.Exception e)
		{
			Debug.LogWarning(e.Message);
		}
	}
}
