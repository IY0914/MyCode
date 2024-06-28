using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SelectAttack : MonoBehaviour
{
    //èÓïÒ
    private GameObject controller;
    public GameObject Image1;
    public GameObject Image2;

    // Start is called before the first frame update
    void Start()
    {
        controller = GameObject.FindGameObjectWithTag("Player");   
    }

    // Update is called once per frame
    void Update()
    {
        if(controller.GetComponent<PlayerController>().specialNumber == 0)
        {
            this.transform.position = Image1.transform.position;
        } 
        else if(controller.GetComponent<PlayerController>().specialNumber == 1)
        {
            this.transform.position = Image2.transform.position;
        }
       
    }
}
