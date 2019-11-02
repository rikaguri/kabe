using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;


public class changemain : MonoBehaviour
{

    [SerializeField] float resultTime       = 15f;

    void Start()
    {
        Invoke("SceneChange", resultTime);
    }

    private void SceneChange()
    {
        SceneManager.LoadScene("main");
    }

}
