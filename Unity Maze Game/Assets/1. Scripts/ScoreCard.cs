using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityStandardAssets;

public class ScoreCard : MonoBehaviour
{
    public Text MyText;
    private int score;

    // Use this for initialization
    void Start()
    {
        MyText.text = "";
    }
    
    // Update is called once per frame
    void Update()
    {

        MyText.text = score + "pts";

    }

    void OnTriggerEnter(Collider coll)
    {
        if (coll.CompareTag("Pickable"))
        {
            score = score + 1;
        }

        if (coll.CompareTag("Pickable"))
        {
            Destroy(coll.gameObject);
        }
    }
}