using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class rotate : MonoBehaviour
{
    private float rotation = 0.0f;

    // Update is called once per frame
    void Update()
    {
        rotation += Time.deltaTime;

        this.gameObject.transform.rotation = Quaternion.Euler(0,(rotation * 10), 0);
    }
}
