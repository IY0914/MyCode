using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DestroyObj : MonoBehaviour
{
    //è¡ñ≈Ç‹Ç≈ÇÃéûä‘
    public float deltaTime = 3.0f;

    //ínñ Ç…ìñÇΩÇ¡ÇƒÇ©ÇÁ3ïbÇ≈è¡ñ≈
    private void OnCollisionEnter(Collision collision)
    {
        if(collision.gameObject.tag == "Ground")
        {
            Destroy(gameObject, deltaTime);
        }
    }

    // Start is called before the first frame update
    void Start()
    {
       Destroy(gameObject, deltaTime);
    }

    private void Update()
    {
        
    }
}
