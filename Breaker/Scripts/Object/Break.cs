using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Break : MonoBehaviour
{
    //ï®óùââéZ
    Rigidbody rigidbody;

    // Start is called before the first frame update
    void Start()
    {
        rigidbody = this.gameObject.GetComponent<Rigidbody>();
    }

    private void OnCollisionEnter(Collision collision)
    {
        if(collision.gameObject.tag == "Weapon")
        {
            rigidbody.isKinematic = false;
        }
        
    }

    // Update is called once per frame
    void Update()
    {
        

        
    }
}
