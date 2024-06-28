using System.Collections;
using System.Collections.Generic;
using UnityEditor;
using UnityEngine;

public class Cursor : MonoBehaviour
{
    public Vector2 pos;

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        pos = Camera.main.ScreenToWorldPoint(Input.mousePosition);
        this.transform.position = pos;
    }
}
