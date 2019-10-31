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
        text = this.GetComponent<Text>();
        text.color = new Color(1.0f, 1.0f, 1.0f, 1.0f);
    }

    // Update is called once per frame
    void Update()
    {

    }



    void OnDataReceived(string message)
    {
        try
        {
            string[] angles = message.Split(',');
            //text.text = "x:" + angles[0] + "\n" + "y:" + angles[1] + "\n" + "z:" + angles[2] + "\n"; // シリアルの値をテキストに表示
            text.text = "angles[0]";
        }
        catch (System.Exception e)
        {
            Debug.LogWarning(e.Message);
        }
    }
}
