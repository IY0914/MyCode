using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DestoryObject : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    private void OnCollisionEnter(Collision collision)
    {
        if(collision.gameObject.name == "Robot")
        {
           
            Destroy(GameObject.Find("Wall"));
        }

        if(collision.gameObject.tag == "Player")
        {
            Destroy(GameObject.Find("RowWall"));
        }
    }
}
