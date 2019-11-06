using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class pink : MonoBehaviour
{
   public SerialHandler serialHandler;
    public Text text;

    // Start is called before the first frame update
    void Start()
    {
		serialHandler.OnDataReceived += OnDataReceived;
		text = this.GetComponent<Text>();
        text.color = new Color(1.0f, 1.0f, 1.0f, 1.0f);
    }

    // Update is called once per frame
    void Update()
    {

    }



	void OnDataReceived(string message)
	{
		var data = message.Split(
				new string[] { "\t" }, System.StringSplitOptions.None);
		if (data.Length < 2) return;

		try
		{
			text.text = data[2];
			//blueScore = int.Parse(data[1]);
		}
		catch (System.Exception e)
		{
			Debug.LogWarning(e.Message);
		}
	}
}
