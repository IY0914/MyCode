using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class StagePush : MonoBehaviour
{
    //ÉvÉåÉCÉÑÅ[Ç™ì•ÇÒÇ≈Ç¢ÇÈÇ©
    public bool playerpush = false;

    private void OnTriggerEnter(Collider other)
    {
        if (other.gameObject.tag == "Player")
        {
            playerpush = true;
        }
    }

    private void OnTriggerExit(Collider other)
    {
        if (other.gameObject.tag == "Player")
        {
            playerpush = false;
        }
    }
}
