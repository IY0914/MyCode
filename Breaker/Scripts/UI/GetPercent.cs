using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;

public class GetPercent : MonoBehaviour
{
    public TextMeshProUGUI text;

    //public static float PersentSum = 10.0f;
    float Persent = 10.0f;

    // Start is called before the first frame update
    void Start()
    {
        //Debug.Log(""+ BreakCheck.PersentSum);
    }
    // Update is called once per frame
    void Update()
    {
        text.text = (int)BreakCheck.PersentSum + "%";
    }


}